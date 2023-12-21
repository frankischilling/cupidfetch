// Cupidfetch is a system information retrieval tool written in C for Linux systems. It's a beginner-friendly, work-in-progress hobby project aimed at learning and exploring programming concepts.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include <pwd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <termios.h>
#include <libgen.h>
#include <stdarg.h>
#include "libs/ini.h"

int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

void print_info(const char *key, const char *format, int align_key, int align_value, ...) {
    va_list args;
    va_start(args, align_value);

    // Print the key with left alignment
    printf("%-*s: ", align_key, key);

    // Print the formatted value with specified alignment
    vprintf(format, args);

    va_end(args);

    // Add a newline at the end
    printf("\n");
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
            char* distroId = strchr(line, '=') + 1;
            distroId[strcspn(distroId, "\n")] = '\0';

            // Convert to lowercase for case-insensitive comparison
            for (int i = 0; distroId[i]; i++) {
                distroId[i] = tolower(distroId[i]);
            }

            // List of supported distros
            const char* supported_distros[] = {
                    "arch", "debian", "ubuntu", "centos", "fedora", "redhat", "opensuse",
                    "gentoo", "alpine", "slackware", "manjaro", "elementary", "mint", "kali",
                    "zorin", "void", "alma", "artix", "endeavouros", "mx", "peppermint",
                    "pop", "solus", "antergos", "mageia", "void", "slackware"
            };

            int supported = 0;

            // Check if the distroId is in the list of supported distros
            for (int i = 0; i < sizeof(supported_distros) / sizeof(supported_distros[0]); i++) {
                if (strstr(distroId, supported_distros[i]) != NULL) {
                    supported = 1;
                    break;
                }
            }

            if (!supported) {
                printf("Warning: Unknown distribution '%s'\n", distroId);
                break; // Continue without updating distro
            }

            // Set the distro based on the detected ID
            if (strstr(distroId, "arch") != NULL) {
                distro = "Arch Linux";
            } else if (strstr(distroId, "debian") != NULL) {
                distro = "Debian";
            } else if (strstr(distroId, "ubuntu") != NULL) {
                distro = "Ubuntu";
            } else if (strstr(distroId, "centos") != NULL) {
                distro = "CentOS";
            } else if (strstr(distroId, "fedora") != NULL) {
                distro = "Fedora";
            } else if (strstr(distroId, "redhat") != NULL) {
                distro = "Red Hat";
            } else if (strstr(distroId, "opensuse") != NULL) {
                distro = "openSUSE";
            } else if (strstr(distroId, "gentoo") != NULL) {
                distro = "Gentoo";
            } else if (strstr(distroId, "alpine") != NULL) {
                distro = "Alpine Linux";
            } else if (strstr(distroId, "slackware") != NULL) {
                distro = "Slackware";
            } else if (strstr(distroId, "manjaro") != NULL) {
                distro = "Manjaro";
            } else if (strstr(distroId, "elementary") != NULL) {
                distro = "elementary OS";
            } else if (strstr(distroId, "mint") != NULL) {
                distro = "Linux Mint";
            } else if (strstr(distroId, "kali") != NULL) {
                distro = "Kali Linux";
            } else if (strstr(distroId, "zorin") != NULL) {
                distro = "Zorin OS";
            } else if (strstr(distroId, "void") != NULL) {
                distro = "Void Linux";
            } else if (strstr(distroId, "alma") != NULL) {
                distro = "AlmaLinux";
            } else if (strstr(distroId, "artix") != NULL) {
                distro = "Artix Linux";
            } else if (strstr(distroId, "endeavouros") != NULL) {
                distro = "EndeavourOS";
            } else if (strstr(distroId, "mx") != NULL) {
                distro = "MX Linux";
            } else if (strstr(distroId, "peppermint") != NULL) {
                distro = "Peppermint OS";
            } else if (strstr(distroId, "pop") != NULL) {
                distro = "Pop!_OS";
            } else if (strstr(distroId, "solus") != NULL) {
                distro = "Solus";
            } else if (strstr(distroId, "antergos") != NULL) {
                distro = "Antergos";
            } else if (strstr(distroId, "mageia") != NULL) {
                distro = "Mageia";
            }

            break;
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
    print_info("Host Name", host_name, 20, 30);
}

const char* get_terminal() {
    const char* term_program = getenv("TERM");
    if (term_program == NULL) {
        // If TERM is not set, return "Unknown"
        return "Unknown";
    }

    // Check for common terminal prefixes and remove them
    const char* prefixes[] = {"xterm", "rxvt", "kitty", "alacritty", "gnome", "terminator", "tmux", /* Add more if needed */};
    size_t num_prefixes = sizeof(prefixes) / sizeof(prefixes[0]);

    for (size_t i = 0; i < num_prefixes; ++i) {
        size_t prefix_len = strlen(prefixes[i]);
        if (strncmp(term_program, prefixes[i], prefix_len) == 0) {
            // Allocate space for the modified terminal name
            size_t term_len = strlen(term_program) - prefix_len;
            char* modified_term = (char*)malloc((term_len + 1) * sizeof(char));

            // Remove the prefix
            strcpy(modified_term, term_program + prefix_len);

            // Remove leading hyphen if present
            if (modified_term[0] == '-') {
                memmove(modified_term, modified_term + 1, term_len);
                term_len--;
            }

            // Check for "-text" suffix and remove it
            size_t suffix_len = strlen("-text");
            if (term_len > suffix_len && strcmp(modified_term + term_len - suffix_len, "-text") == 0) {
                modified_term[term_len - suffix_len] = '\0';
            }

            return modified_term;
        }
    }

    // If no recognized prefix is found, return the original TERM value
    return term_program;
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

    print_info("Shell", path, 20, 30);
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

    print_info("DE", path, 20, 30);
    pclose(fp);
}

void get_username() {
    char* username = getlogin();
    if (username != NULL) {
        print_info("Username", username, 20, 30);
    } else {
        printf("Error fetching username\n");
    }
}

void get_package_count(const char* distro) {
    const char* package_command;

    // List of supported distros
    const char* supported_distros[] = {
            "Ubuntu", "Debian", "elementary OS", "Zorin OS", "Pop!_OS", "MX Linux",
            "Kali Linux", "Parrot OS", "Raspbian", "Raspberry Pi OS",
            "Arch Linux", "Manjaro", "Artix Linux", "BlueStar", "EndeavourOS",
            "CentOS", "Fedora", "openSUSE", "Mageia", "AlmaLinux", "Rocky Linux", "Amazon Linux",
            "Gentoo", "Alpine Linux", "Void Linux",
            "Slackware", "Solus",
            "NixOS", "Antergos", "Linux Mint", "Peppermint OS"
    };

    int supported = 0;

    // Check if the provided distro is in the list of supported distros
    for (int i = 0; i < sizeof(supported_distros) / sizeof(supported_distros[0]); i++) {
        if (strcmp(distro, supported_distros[i]) == 0) {
            supported = 1;
            break;
        }
    }

    if (!supported) {
        printf("Error: Unknown distribution '%s'\n", distro);
        return; // Return without running the command
    }

    // Set the package command based on the distro
    if (strcmp(distro, "Ubuntu") == 0 || strcmp(distro, "Debian") == 0 || strcmp(distro, "elementary OS") == 0 || strcmp(distro, "Zorin OS") == 0 || strcmp(distro, "Pop!_OS") == 0 || strcmp(distro, "MX Linux") == 0 || strcmp(distro, "Kali Linux") == 0 || strcmp(distro, "Parrot OS") == 0 || strcmp(distro, "Raspbian") == 0 || strcmp(distro, "Raspberry Pi OS") == 0) {
        package_command = "dpkg -l | tail -n+6 | wc -l";
    } else if (strcmp(distro, "Arch Linux") == 0 || strcmp(distro, "Manjaro") == 0 || strcmp(distro, "Artix Linux") == 0 || strcmp(distro, "BlueStar") == 0 || strcmp(distro, "EndeavourOS") == 0) {
        package_command = "pacman -Q | wc -l";
    } else if (strcmp(distro, "CentOS") == 0 || strcmp(distro, "Fedora") == 0 || strcmp(distro, "openSUSE") == 0 || strcmp(distro, "Mageia") == 0 || strcmp(distro, "AlmaLinux") == 0 || strcmp(distro, "Rocky Linux") == 0 || strcmp(distro, "Amazon Linux") == 0) {
        package_command = "rpm -qa | wc -l";
    } else if (strcmp(distro, "Gentoo") == 0) {
        package_command = "equery -q list '*' | wc -l";
    } else if (strcmp(distro, "Alpine Linux") == 0) {
        package_command = "apk info | wc -l";
    } else if (strcmp(distro, "Void Linux") == 0) {
        package_command = "xbps-query -l | wc -l";
    } else if (strcmp(distro, "Slackware") == 0) {
        package_command = "ls /var/log/packages/ | wc -l";
    } else if (strcmp(distro, "Solus") == 0) {
        package_command = "eopkg list-installed | wc -l";
    } else if (strcmp(distro, "NixOS") == 0) {
        package_command = "nix-env --list-generations | wc -l";
    } else if (strcmp(distro, "Antergos") == 0) {
        package_command = "pacman -Q | wc -l";
    } else if (strcmp(distro, "Linux Mint") == 0) {
        package_command = "dpkg -l | tail -n+6 | wc -l";
    } else if (strcmp(distro, "Peppermint OS") == 0) {
        package_command = "dpkg -l | tail -n+6 | wc -l";
    }

    // Run the package command and display the result
    FILE* fp = popen(package_command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(EXIT_FAILURE);
    }

    char output[100];
    if (fgets(output, sizeof(output), fp) != NULL) {
        output[strcspn(output, "\n")] = 0; // Remove newline character if present
        print_info("Package Count", output, 20, 30);
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

    print_info("Linux Kernel", uname_data.release, 20, 30);
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

    // Corrected usage of print_info
    print_info("Uptime", "%d days, %02d:%02d", 20, 30, days, hours, minutes);
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

            // Print with dynamic padding for right alignment
            print_info("Local IP", "%s", 20, 30, ip_addr);
            break;
        }
    }

    freeifaddrs(ifaddr);
}

void get_available_memory() {
    struct sysinfo sys_info;
    if (sysinfo(&sys_info) != 0) {
        fprintf(stderr, "Error getting system information\n");
        exit(EXIT_FAILURE);
    }

    // Convert bytes to megabytes for readability
    unsigned long total_memory_mb = sys_info.totalram / (1024 * 1024);
    unsigned long free_memory_mb = sys_info.freeram / (1024 * 1024);

    print_info("Memory", "%luMB/%luMB", 20, 30, free_memory_mb, total_memory_mb);
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

void display_available_memory() {
    get_available_memory();
}

struct CupidConfig {
    int display_host_name;
    int display_username;
    int display_distro;
    int display_linux_kernel;
    int display_uptime;
    int display_package_count;
    int display_shell;
    int display_terminal;
    int display_desktop_environment;
    int display_local_ip;
    int display_available_memory;
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
    fprintf(config_file, "available_memory = %d\n", default_config->display_available_memory);

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
    fprintf(config_file, "terminal = %d\n", user_config->display_terminal);
    fprintf(config_file, "desktop_environment = %d\n", user_config->display_desktop_environment);
    fprintf(config_file, "local_ip = %d\n", user_config->display_local_ip);
    fprintf(config_file, "available_memory = %d\n", user_config->display_available_memory);

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
        } else if (strcmp(name, "terminal") == 0) {
            config->display_terminal = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "desktop_environment") == 0) {
            config->display_desktop_environment = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "local_ip") == 0) {
            config->display_local_ip = (value != NULL) ? atoi(value) : 1;
        } else if (strcmp(name, "available_memory") == 0) {
            config->display_available_memory = (value!= NULL)? atoi(value) : 1;
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
            .display_terminal = 1,
            .display_desktop_environment = 1,
            .display_local_ip = 1,
            .display_available_memory = 1,
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

    if (mkdir(configDirPath, 0700) != 0 && errno != EEXIST) {
        // If mkdir failed and the error is not EEXIST, try creating parent directories
        char parentDirPath[256];
        snprintf(parentDirPath, sizeof(parentDirPath), "%s/%s", homeDir, ".config");
        if (mkdir(parentDirPath, 0700) != 0 && errno != EEXIST) {
            fprintf(stderr, "Error creating config directory: %s\n", configDirPath);
            perror("mkdir");
            exit(EXIT_FAILURE);
        }

        // Retry creating the config directory
        if (mkdir(configDirPath, 0700) != 0 && errno != EEXIST) {
            fprintf(stderr, "Error creating config directory: %s\n", configDirPath);
            perror("mkdir");
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
        print_info("Distro", detectedDistro, 20, 30);
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
    if (userConfig.display_terminal) {
        const char* terminal_program = get_terminal();
        print_info("Terminal", "%s", 20, 30, terminal_program);
    }
    if (userConfig.display_desktop_environment) {
        get_desktop_environment();
    }
    if (userConfig.display_local_ip) {
        display_local_ip();
    }
    if (userConfig.display_available_memory) {
        get_available_memory();  // Call get_available_memory only if the option is set to 1
    }

    return 0;
}
