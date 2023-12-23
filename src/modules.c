#include "cupidfetch.h"

void get_hostname() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        fprintf(stderr, "Error getting hostname\n");
        exit(EXIT_FAILURE);
    }
    print_info("Hostname", hostname, 20, 30);
}

void get_username() {
    char* username = getlogin();
    if (username != NULL) {
        print_info("Username", username, 20, 30);
    } else {
        printf("Error fetching username\n");
    }
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

struct DistroPackageInfo {
    const char* distro;
    const char* package_command;
};

void get_package_count(const char* distro) {
    const char* package_command = NULL;

    // List of supported distros and their package commands
    struct DistroPackageInfo supported_distros[] = {
            {"Ubuntu", "dpkg -l | tail -n+6 | wc -l"},
            {"Debian", "dpkg -l | tail -n+6 | wc -l"},
            {"elementary OS", "dpkg -l | tail -n+6 | wc -l"},
            {"Zorin OS", "dpkg -l | tail -n+6 | wc -l"},
            {"Pop!_OS", "dpkg -l | tail -n+6 | wc -l"},
            {"MX Linux", "dpkg -l | tail -n+6 | wc -l"},
            {"Kali Linux", "dpkg -l | tail -n+6 | wc -l"},
            {"Parrot OS", "dpkg -l | tail -n+6 | wc -l"},
            {"Raspbian", "dpkg -l | tail -n+6 | wc -l"},
            {"Raspberry Pi OS", "dpkg -l | tail -n+6 | wc -l"},
            {"Arch Linux", "pacman -Q | wc -l"},
            {"Manjaro", "pacman -Q | wc -l"},
            {"Artix Linux", "pacman -Q | wc -l"},
            {"BlueStar", "pacman -Q | wc -l"},
            {"EndeavourOS", "pacman -Q | wc -l"},
            {"CentOS", "rpm -qa | wc -l"},
            {"Fedora", "rpm -qa | wc -l"},
            {"openSUSE", "rpm -qa | wc -l"},
            {"Mageia", "rpm -qa | wc -l"},
            {"AlmaLinux", "rpm -qa | wc -l"},
            {"Rocky Linux", "rpm -qa | wc -l"},
            {"Amazon Linux", "rpm -qa | wc -l"},
            {"Gentoo", "equery -q list '*' | wc -l"},
            {"Alpine Linux", "apk info | wc -l"},
            {"Void Linux", "xbps-query -l | wc -l"},
            {"Slackware", "ls /var/log/packages/ | wc -l"},
            {"Solus", "eopkg list-installed | wc -l"},
            {"NixOS", "nix-env --list-generations | wc -l"},
            {"Antergos", "pacman -Q | wc -l"},
            {"Linux Mint", "dpkg -l | tail -n+6 | wc -l"},
            {"Peppermint OS", "dpkg -l | tail -n+6 | wc -l"}
            // Add more distros as needed
    };

    // Find the package command for the provided distro
    for (int i = 0; i < sizeof(supported_distros) / sizeof(supported_distros[0]); i++) {
        if (strcmp(distro, supported_distros[i].distro) == 0) {
            package_command = supported_distros[i].package_command;
            break;
        }
    }

    if (package_command == NULL) {
        printf("Error: Unsupported distribution '%s'\n", distro);
        return; // Return without running the command
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
        print_info("Package Count", "%s", 20, 30, output); // Corrected usage
    } else {
        printf("Error reading package count\n");
    }

    pclose(fp);
}

void get_shell() {
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);

    if (pw != NULL) {
        // Extract the shell from the password file entry
        const char *shell = pw->pw_shell;

        if (shell != NULL) {
            // Extract the base name of the shell
            const char *baseName = strrchr(shell, '/');
            baseName = (baseName != NULL) ? baseName + 1 : shell;

            print_info("Shell", baseName, 20, 30);
            return;
        }
    }

    // Fallback: Print unknown if shell cannot be determined
    print_info("Shell", "Unknown", 20, 30);
}

const char* get_terminal() {
    const char* term_program = getenv("TERM");

    if (term_program != NULL) {
        // Get the terminal type from the TERM environment variable
        return term_program;
    } else {
        if (isatty(STDOUT_FILENO)) {
            // Check if stdout is a terminal
            struct winsize w;
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
                return "Unknown (Interactive)";
            } else {
                return "Unknown (Non-Interactive)";
            }
        } else {
            return "Not a terminal";
        }
    }
}

void get_desktop_environment() {
    // Try to get desktop environment using $XDG_CURRENT_DESKTOP
    const char* xdgDesktop = getenv("XDG_CURRENT_DESKTOP");
    if (xdgDesktop != NULL && strlen(xdgDesktop) > 0) {
        print_info("DE", xdgDesktop, 20, 30);
        return; // Successfully retrieved desktop environment
    }

    // If $XDG_CURRENT_DESKTOP is empty or not found, try checking processes

    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error opening /proc directory");
        return;
    }

    int desktopFound = 0;  // Flag to indicate if a desktop environment has been found

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[270];
            snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);

            FILE *cmdlineFile = fopen(path, "r");
            if (cmdlineFile != NULL) {
                char cmdline[256];
                if (fgets(cmdline, sizeof(cmdline), cmdlineFile) != NULL) {

                    // Example: Check if "gnome-shell" is in the cmdline
                    if (strstr(cmdline, "gnome-shell") != NULL) {
                        print_info("DE", "GNOME", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    // Example: Check if "plasma-desktop" is in the cmdline
                    if (strstr(cmdline, "plasma-desktop") != NULL) {
                        print_info("DE", "KDE Plasma", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    // Example: Check if "fvwm" is in the cmdline
                    if (strstr(cmdline, "fvwm") != NULL) {
                        print_info("DE", "FVWM", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "xfce4-session") != NULL) {
                        print_info("DE", "XFCE", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "mate-session") != NULL) {
                        print_info("DE", "MATE", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "lxqt-session") != NULL) {
                        print_info("DE", "LXQt", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "lxde-session") != NULL) {
                        print_info("DE", "LXDE", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "i3") != NULL) {
                        print_info("DE", "i3", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "awesome") != NULL) {
                        print_info("DE", "Awesome", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "bspwm") != NULL) {
                        print_info("DE", "bspwm", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "dwm") != NULL) {
                        print_info("DE", "dwm", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                    if (strstr(cmdline, "hyprland") != NULL) {
                        print_info("DE", "hyprland", 20, 30);
                        desktopFound = 1;
                        break;
                    }

                }

                fclose(cmdlineFile);
            }
        }
    }

    closedir(dir);

    // If no desktop environment is detected, print Unknown
    if (!desktopFound) {
        print_info("DE", "Unknown", 20, 30);
    }
}

void get_local_ip() {
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
    // Linux-specific implementation
    // Source: https://github.com/KittyKatt/screenFetch/issues/386#issuecomment-249312716
    // Also used in neofetch
    ssize_t mem_avail = -1, mem_total = -1;
    long mem_used = 0;
    FILE* meminfo;
    char line[LINUX_PROC_LINE_SZ];

    meminfo = fopen("/proc/meminfo", "r");

    if (meminfo == NULL) {
        fprintf(stderr, "Error: Failed to open /proc/meminfo\n");
        return;
    }

    while (fgets(line, sizeof line, meminfo)) {
        char *value = NULL;
        size_t vnum;
        size_t i, len = 0, vlen = 0;

        for (i = 0; line[i]; i++) {
            if (!len && line[i] == ':')
                len = i;
            if (len && !value && isdigit(line[i]))
                value = &line[i];
            if (len && value && isdigit(line[i]))
                vlen = 1 + &line[i] - value;
        }
        if (!len || !vlen || !value)
            continue;

        line[len] = '\0';
        value[vlen] = '\0';

        if (1 != sscanf(value, "%zu", &vnum))
            continue;

        if (0 == strcmp("MemTotal", line)) {
            mem_total = vnum;
            mem_used += vnum;
        } else if (0 == strcmp("MemAvailable", line))
            mem_avail = vnum;
        else if (0 == strcmp("Shmem", line))
            mem_used += vnum;
        else if (0 == strcmp("MemFree", line))
            mem_used -= vnum;
        else if (0 == strcmp("Buffers", line))
            mem_used -= vnum;
        else if (0 == strcmp("Cached", line))
            mem_used -= vnum;
        else if (0 == strcmp("SReclaimable", line))
            mem_used -= vnum;

        if (mem_total != -1 && mem_avail != -1)
            break;
    }

    fclose(meminfo);

    if (mem_total == -1) {
        fprintf(stderr, "Error: Failed to retrieve total memory.\n");
        return;
    }

    if (mem_avail != -1) {
        mem_used = mem_total - mem_avail;
    }

    print_info(
            "Memory", "%ld %s / %ld %s", 20, 30,
            mem_used * 1024 / g_userConfig.memory_unit_size,
	    g_userConfig.memory_unit,
	    mem_total * 1024 / g_userConfig.memory_unit_size,
        g_userConfig.memory_unit
    );
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



