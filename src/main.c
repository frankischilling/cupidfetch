#include "cupidfetch.h"


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

void display_fetch() {
    // Fetch system information
    const char* detectedDistro = detect_linux_distro();
    char hostname[256];
    char* username = getlogin();

    // Check for errors getting host name
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        fprintf(stderr, "Error getting host name\n");
        exit(EXIT_FAILURE);
    }

    // Calculate alignment spaces
    int artWidth = 18;
    int totalWidth = 50;
    int textWidth = strlen(username) + strlen(hostname) + 1;
    int spaces = totalWidth - artWidth - textWidth;

    // Display username@hostname
    printf("\n%*s%s@%s\n", spaces, "", username, hostname);

    // Display cat ASCII art based on the detected distribution
    print_cat(detectedDistro);

    // Display system information based on user configuration
    printf("\n");
    printf("-----------------------------------------\n");

    // Display system information based on loaded or default user configuration
    if (g_userConfig.display_host_name)           get_hostname();
    if (g_userConfig.display_username)            get_username();
    if (g_userConfig.display_distro)              print_info("Distro", detectedDistro, 20, 30);
    if (g_userConfig.display_linux_kernel)        get_linux_kernel();
    if (g_userConfig.display_uptime)              get_uptime();
    if (g_userConfig.display_package_count)       get_package_count(detectedDistro);
    if (g_userConfig.display_shell)               get_shell();
    if (g_userConfig.display_terminal)            print_info("Terminal", "%s", 20, 30, get_terminal());
    if (g_userConfig.display_desktop_environment) get_desktop_environment();
    if (g_userConfig.display_local_ip)            get_local_ip();
    if (g_userConfig.display_available_memory)    get_available_memory();
    if (g_userConfig.display_available_storage)   get_available_storage();
}

int main() {
    int parse_result = 0;

    init_g_config();

    if (!isatty(STDIN_FILENO)) {
        parse_result = ini_parse_file(stdin, cupid_ini_handler, &g_userConfig);
        goto CONFIGURED;
    }

    // Why is obvious stuff commented but the confusing parts are not?
    // Determine the home directory of the current user
    const char* home = get_home_directory();

    // Construct the path for the config file
    char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/.config/cupidfetch/cupidfetch.ini", home);

    // Config file exists, load configuration from the file
    parse_result = ini_parse(config_path, cupid_ini_handler, &g_userConfig);

CONFIGURED:


    if (parse_result < 0) {
        fprintf(stderr, "Error parsing INI file: %s\n", strerror(parse_result));
        exit(EXIT_FAILURE);
    }

    display_fetch();

    return 0;
}
