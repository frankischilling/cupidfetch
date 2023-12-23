#include "cupidfetch.h"

struct CupidConfig g_userConfig;

// Define a structure to hold distro information
struct DistroInfo {
    const char* shortname;
    const char* longname;
};

const char* detect_linux_distro() {
    FILE* os_release = fopen("/etc/os-release", "r");
    if (os_release == NULL) {
        fprintf(stderr, "Error opening /etc/os-release\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
    const char* distro = "Unknown";

    // Define an array of DistroInfo structs for supported distros
    // TODO: get this from the config
    const struct DistroInfo supported_distros[] = {
            {"arch", "Arch Linux"},
            {"debian", "Debian"},
            {"ubuntu", "Ubuntu"},
            {"centos", "CentOS"},
            {"fedora", "Fedora"},
            {"redhat", "Red Hat"},
            {"opensuse", "openSUSE"},
            {"gentoo", "Gentoo"},
            {"alpine", "Alpine Linux"},
            {"slackware", "Slackware"},
            {"manjaro", "Manjaro"},
            {"elementary", "elementary OS"},
            {"mint", "Linux Mint"},
            {"kali", "Kali Linux"},
            {"zorin", "Zorin OS"},
            {"void", "Void Linux"},
            {"alma", "AlmaLinux"},
            {"artix", "Artix Linux"},
            {"endeavouros", "EndeavourOS"},
            {"mx", "MX Linux"},
            {"peppermint", "Peppermint OS"},
            {"pop", "Pop!_OS"},
            {"solus", "Solus"},
            {"antergos", "Antergos"},
            {"mageia", "Mageia"}
            // Add more distros as needed
    };

    while (fgets(line, sizeof(line), os_release)) {
        if (strstr(line, "ID=") != NULL) {
            char* distroId = strchr(line, '=') + 1;
            size_t len = strlen(distroId);

            // Remove trailing newline character if present
            if (len > 0 && distroId[len - 1] == '\n') {
                distroId[len - 1] = '\0';
            }

            // Convert to lowercase for case-insensitive comparison
            for (size_t i = 0; distroId[i]; i++) {
                distroId[i] = tolower(distroId[i]);
            }

            // Check if the distroId is in the list of supported distros
            int supported = 0;
            for (size_t i = 0; i < sizeof(supported_distros) / sizeof(supported_distros[0]); i++) {
                if (strstr(distroId, supported_distros[i].shortname) != NULL) {
                    supported = 1;
                    distro = supported_distros[i].longname;
                    break;
                }
            }

            if (!supported) {
                printf("Warning: Unknown distribution '%s'\n", distroId);
                break; // Continue without updating distro
            }

            break;
        }
    }

    fclose(os_release);
    return distro;
}

int main() {
    // Declare and initialize the configuration
    struct CupidConfig cfg_ = {
            .display_host_name = 1,
            .display_username = 1,
            .display_distro = 1,
            .display_linux_kernel = 1,
            .display_uptime = 1,
            .display_package_count = 1,
            .display_shell = 1,
            .display_terminal = 1,
            .display_desktop_environment = 1,
            .display_window_manager = 1,
            .display_local_ip = 1,
            .display_available_memory = 1,
	    .memory_unit = "MiB",
	    .memory_unit_size = 1024 * 1024,
    };
    g_userConfig = cfg_;

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
        int parse_result = ini_parse(configPath, iniHandler, &g_userConfig);
        if (parse_result < 0) {
            fprintf(stderr, "Error parsing INI file: %s\n", strerror(parse_result));
            exit(EXIT_FAILURE);
        }

        // Update the configuration with new options
        g_userConfig.display_terminal = 1;  // Add your new options here

        // Save the updated configuration back to the file
        FILE* config_file = fopen(configPath, "w");
        if (config_file == NULL) {
            fprintf(stderr, "Error updating config file: %s\n", configPath);
            exit(EXIT_FAILURE);
        }

        fclose(config_file);
    } else {
        // Config file doesn't exist, create the default configuration file
        create_default_config(configPath, &g_userConfig);
    }


    // Display system information based on loaded or default user configuration
    if (g_userConfig.display_host_name) {
        get_hostname();
    }
    if (g_userConfig.display_username) {
        get_username();
    }
    if (g_userConfig.display_distro) {
        print_info("Distro", detectedDistro, 20, 30);
    }
    if (g_userConfig.display_linux_kernel) {
        get_linux_kernel();
    }
    if (g_userConfig.display_uptime) {
        get_uptime();
    }
    if (g_userConfig.display_package_count) {
        get_package_count(detectedDistro);
    }
    if (g_userConfig.display_shell) {
        get_shell();
    }
    if (g_userConfig.display_terminal) {
        const char* terminal_program = get_terminal();
        print_info("Terminal", "%s", 20, 30, terminal_program);
    }
    if (g_userConfig.display_desktop_environment) {
        get_desktop_environment();
    }
    if (g_userConfig.display_window_manager) {
        get_window_manager();
    }
    if (g_userConfig.display_local_ip) {
        get_local_ip();
    }
    if (g_userConfig.display_available_memory) {
        get_available_memory();  // Call get_available_memory only if the option is set to 1
    }

    return 0;
}
