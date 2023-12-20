// Cupidfetch is a system information retrieval tool written in C for Linux systems. It's a beginner-friendly, work-in-progress hobby project aimed at learning and exploring programming concepts.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <pwd.h>
#include <sys/stat.h>
#include "libs/ini.h"

int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

void print_info(const char *key, const char *value) {
    int key_length = strlen(key);
    int padding = 30 - key_length; // Adjust the padding as needed for alignment
    if (strcmp(key, "Local IP") == 0) {
        printf("%s%*s: %s\n", key, padding - 2, "", value); // Adjusted padding for "Local IP"
    } else {
        printf("%-15s: %s\n", key, value);
    }
}

void print_cat(const char* distro) {
    if (strcmp(distro, "Ubuntu") == 0 || strcmp(distro, "Debian") == 0 ||
        strcmp(distro, "Arch") == 0 || strcmp(distro, "CentOS") == 0) {
        printf(" /\\_/\\\n");
        printf("( o.o )\n");
        printf(" > ^ <  cupidfetch");
    } else {
        printf(" /\\_/\\\n");
        printf("( x.x )\n");
        printf(" > ^ <  cupidfetch");
    }
}

const char* detect_linux_distro() {
    FILE* os_release = fopen("/etc/os-release", "r");
    if (os_release == NULL) {
        fprintf(stderr, "Error opening /etc/os-release\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
    const char* distro = "Unknown";

    while (fgets(line, sizeof(line), os_release)) {
        if (strstr(line, "ID=") != NULL) {
            if (strstr(line, "ubuntu") != NULL) {
                distro = "Ubuntu";
                break;
            } else if (strstr(line, "debian") != NULL) {
                distro = "Debian";
                break;
            } else if (strstr(line, "arch") != NULL) {
                distro = "Arch";
                break;
            } else if (strstr(line, "centos") != NULL) {
                distro = "CentOS";
                break;
            }
        }
    }

    fclose(os_release);
    return distro;
}


void get_host_name() {
    char host_name[256];
    if (gethostname(host_name, sizeof(host_name)) != 0) {
        fprintf(stderr, "Error getting host name\n");
        exit(EXIT_FAILURE);
    }
    print_info("Host Name", host_name);
}

void get_shell() {
    FILE *fp;
    char path[1035];

    fp = popen("basename $SHELL", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(EXIT_FAILURE);
    }

    fgets(path, sizeof(path) - 1, fp);
    if (strlen(path) > 0) {
        path[strlen(path) - 1] = '\0'; // Remove newline character
    }

    print_info("Shell", path);
    pclose(fp);
}

void get_desktop_environment() {
    FILE *fp;
    char path[1035];

    fp = popen("echo $XDG_CURRENT_DESKTOP", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(EXIT_FAILURE);
    }

    fgets(path, sizeof(path) - 1, fp);
    if (strlen(path) > 0) {
        path[strlen(path) - 1] = '\0'; // Remove newline character
    }

    print_info("DE", path);
    pclose(fp);
}

void get_username() {
    char* username = getlogin();
    if (username != NULL) {
        print_info("Username", username);
    } else {
        printf("Error fetching username\n");
    }
}

void get_package_count(const char* distro) {
    const char* package_command;

    if (strcmp(distro, "Ubuntu") == 0 || strcmp(distro, "Debian") == 0) {
        package_command = "dpkg -l | tail -n+6 | wc -l";
    } else if (strcmp(distro, "Arch") == 0) {
        package_command = "pacman -Q | wc -l";
    } else if (strcmp(distro, "CentOS") == 0) {
        package_command = "rpm -qa | wc -l";
    } else {
        package_command = "Unknown distribution";
    }

    FILE* fp = popen(package_command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(EXIT_FAILURE);
    }

    char output[100];
    if (fgets(output, sizeof(output), fp) != NULL) {
        output[strcspn(output, "\n")] = 0; // Remove newline character if present
        print_info("Package Count", output);
    } else {
        printf("Error reading package count\n");
    }

    pclose(fp);
}

void get_linux_kernel() {
    struct utsname uname_data;
    if (uname(&uname_data) != 0) {
        fprintf(stderr, "Error getting Linux kernel information\n");
        exit(EXIT_FAILURE);
    }

    print_info("Linux Kernel", uname_data.release);
}

void get_uptime() {
    FILE* uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL) {
        fprintf(stderr, "Error fetching uptime\n");
        exit(EXIT_FAILURE);
    }

    double uptime;
    if (fscanf(uptime_file, "%lf", &uptime) != 1) {
        fprintf(stderr, "Error reading uptime\n");
        exit(EXIT_FAILURE);
    }
    fclose(uptime_file);

    int days = (int)uptime / (60 * 60 * 24);
    int hours = ((int)uptime % (60 * 60 * 24)) / (60 * 60);
    int minutes = ((int)uptime % (60 * 60)) / 60;

    printf("Uptime         : %d days, %02d:%02d\n", days, hours, minutes);
}

void display_local_ip() {
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        printf("Error retrieving IP address.\n");
        return;
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
            char ip_addr[16]; // Allocate enough space for an IPv6 address
            snprintf(ip_addr, sizeof(ip_addr), "%s", inet_ntoa(addr->sin_addr));

            // Check if loopback interface and skip if needed
            if (strcmp(ifa->ifa_name, "lo") == 0) {
                continue;
            }

            // Calculate exact padding based on string lengths
            int key_length = strlen("Local IP");
            int value_length = strlen(ip_addr);
            int padding = 28 - (key_length + value_length); // Adjust as needed

            // Print with dynamic padding for right alignment
            printf("%s%*s: %s\n", "Local IP", padding, "", ip_addr);
            break;
        }
    }

    freeifaddrs(ifaddr);
}

void display_host_name() {
    get_host_name();
}

void display_shell() {
    get_shell();
}

void display_desktop_environment() {
    get_desktop_environment();
}

void display_package_count(const char* distro) {
    get_package_count(distro);
}

void display_linux_kernel() {
    get_linux_kernel();
}

void display_uptime() {
    get_uptime();
}

const char* get_home_directory() {
    const char* homeDir;
    if ((homeDir = getenv("HOME")) == NULL) {
        struct passwd* pw = getpwuid(getuid());
        if (pw != NULL) {
            homeDir = pw->pw_dir;
        }
    }
    return homeDir;
}

struct CupidConfig {
    int display_host_name;
    int display_username;
    int display_distro;
    int display_linux_kernel;
    int display_uptime;
    int display_package_count;
    int display_shell;
    int display_desktop_environment;
    int display_local_ip;
};

void create_default_config(const char* config_path, const struct CupidConfig* default_config) {
    FILE* config_file = fopen(config_path, "w");
    if (config_file == NULL) {
        fprintf(stderr, "Error creating config file: %s\n", config_path);
        exit(EXIT_FAILURE);
    }

    fprintf(config_file, "[DisplayOptions]\n");
    fprintf(config_file, "host_name = %d\n", default_config->display_host_name);
    fprintf(config_file, "username = %d\n", default_config->display_username);
    fprintf(config_file, "distro = %d\n", default_config->display_distro);
    fprintf(config_file, "linux_kernel = %d\n", default_config->display_linux_kernel);
    fprintf(config_file, "uptime = %d\n", default_config->display_uptime);
    fprintf(config_file, "package_count = %d\n", default_config->display_package_count);
    fprintf(config_file, "shell = %d\n", default_config->display_shell);
    fprintf(config_file, "desktop_environment = %d\n", default_config->display_desktop_environment);
    fprintf(config_file, "local_ip = %d\n", default_config->display_local_ip);

    fclose(config_file);
}

void write_config_to_file(const char* config_path, const struct CupidConfig* user_config) {
    FILE* config_file = fopen(config_path, "w");
    if (config_file == NULL) {
        fprintf(stderr, "Error writing to config file: %s\n", config_path);
        exit(EXIT_FAILURE);
    }

    fprintf(config_file, "[DisplayOptions]\n");
    fprintf(config_file, "host_name = %d\n", user_config->display_host_name);
    fprintf(config_file, "username = %d\n", user_config->display_username);
    fprintf(config_file, "distro = %d\n", user_config->display_distro);
    fprintf(config_file, "linux_kernel = %d\n", user_config->display_linux_kernel);
    fprintf(config_file, "uptime = %d\n", user_config->display_uptime);
    fprintf(config_file, "package_count = %d\n", user_config->display_package_count);
    fprintf(config_file, "shell = %d\n", user_config->display_shell);
    fprintf(config_file, "desktop_environment = %d\n", user_config->display_desktop_environment);
    fprintf(config_file, "local_ip = %d\n", user_config->display_local_ip);

    fclose(config_file);
}

// INI handler function
int iniHandler(void* user, const char* section, const char* name, const char* value) {
    struct CupidConfig* config = (struct CupidConfig*)user;

    // Check the section and update corresponding display options
    if (strcmp(section, "DisplayOptions") == 0) {
        if (strcmp(name, "host_name") == 0) {
            config->display_host_name = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "username") == 0) {
            config->display_username = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "distro") == 0) {
            config->display_distro = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "linux_kernel") == 0) {
            config->display_linux_kernel = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "uptime") == 0) {
            config->display_uptime = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "package_count") == 0) {
            config->display_package_count = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "shell") == 0) {
            config->display_shell = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "desktop_environment") == 0) {
            config->display_desktop_environment = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "local_ip") == 0) {
            config->display_local_ip = (value != NULL) ? atoi(value) : 1;
        }
    }

    return 1;
}

int main() {
    // Declare and initialize the configuration
    struct CupidConfig userConfig = {
            .display_host_name = 1,
            .display_username = 1,
            .display_distro = 1,
            .display_linux_kernel = 1,
            .display_uptime = 1,
            .display_package_count = 1,
            .display_shell = 1,
            .display_desktop_environment = 1,
            .display_local_ip = 1,
    };

    // Determine the home directory of the current user
    const char* homeDir = get_home_directory();

    // Construct the path for the config file
    char configPath[256];
    snprintf(configPath, sizeof(configPath), "%s/.config/cupidfetch/cupidfetch.ini", homeDir);

    // Check if the config directory exists, if not, create it
    const char* configDir = ".config/cupidfetch";
    char configDirPath[256];
    snprintf(configDirPath, sizeof(configDirPath), "%s/%s", homeDir, configDir);

    if (access(configDirPath, F_OK) == -1) {
        // Directory does not exist, create it
        if (mkdir(configDirPath, 0700) != 0) {
            fprintf(stderr, "Error creating config directory: %s\n", configDirPath);
            exit(EXIT_FAILURE);
        }
    }

    // Fetch system information
    const char* detectedDistro = detect_linux_distro();
    char hostName[256];
    char* username = getlogin();

    // Check for errors getting host name
    if (gethostname(hostName, sizeof(hostName)) != 0) {
        fprintf(stderr, "Error getting host name\n");
        exit(EXIT_FAILURE);
    }

    // Calculate alignment spaces
    int artWidth = 18;
    int totalWidth = 50;
    int textWidth = strlen(username) + strlen(hostName) + 1;
    int spaces = totalWidth - artWidth - textWidth;

    // Display username@hostname
    printf("\n%*s%s@%s\n", spaces, "", username, hostName);

    // Display cat ASCII art based on the detected distribution
    print_cat(detectedDistro);

    // Display system information based on user configuration
    printf("\n");
    printf("-----------------------------------------\n");

    // Check if the config file exists
    if (access(configPath, F_OK) != -1) {
        // Config file exists, load configuration from the file
        int parse_result = ini_parse(configPath, iniHandler, &userConfig);
        if (parse_result < 0) {
            fprintf(stderr, "Error parsing INI file: %s\n", strerror(parse_result));
            exit(EXIT_FAILURE);
        }
    } else {
        // Config file doesn't exist, create the default configuration file
        create_default_config(configPath, &userConfig);
    }

    // Display system information based on loaded or default user configuration
    if (userConfig.display_host_name) {
        display_host_name();
    }
    if (userConfig.display_username) {
        get_username();
    }
    if (userConfig.display_distro) {
        print_info("Distro", detectedDistro);
    }
    if (userConfig.display_linux_kernel) {
        display_linux_kernel();
    }
    if (userConfig.display_uptime) {
        display_uptime();
    }
    if (userConfig.display_package_count) {
        display_package_count(detectedDistro);
    }
    if (userConfig.display_shell) {
        display_shell();
    }
    if (userConfig.display_desktop_environment) {
        get_desktop_environment();
    }
    if (userConfig.display_local_ip) {
        display_local_ip();
    }

    return 0;
}
