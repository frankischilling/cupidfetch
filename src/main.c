#include "cupidfetch.h"

FILE *g_log = NULL;



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

	    #define DISTRO(shortname, longname, pkgcmd) else if (strstr(distroId, (shortname)) != NULL){\
	    	supported = 1;\
		distro = (longname);}

	    if (0) {}
	    #include "../data/distros.def"


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

    for (size_t i = 0; g_userConfig.modules[i]; i++) {
        g_userConfig.modules[i]();
    }
}

int main() {
    int parse_result = -69;

    init_g_config();
    g_log = NULL;


    if (!isatty(STDIN_FILENO))
        parse_result = ini_parse_file(stdin, cupid_ini_handler, &g_userConfig);
    if (!isatty(STDERR_FILENO))
    	g_log = stderr;

    if (g_log == NULL) {
        char config_path[CONFIG_PATH_SIZE];

	/*
	 * $XDG_CONFIG_HOME defines the base directory relative to which
	 * user-specific configuration files should be stored. If
	 * $XDG_CONFIG_HOME is either not set or empty, a default equal to
	 * $HOME/.config should be used.
	 *
	 * https://www.freedesktop.org/wiki/Specifications/basedir-spec/
	 */

	char *config = NULL;
	if ((config = getenv("XDG_CONFIG_HOME"))) {
		snprintf(config_path, sizeof(config_path), "%s/cupidfetch/log.txt", config);
	} else {
		const char* home = get_home_directory();
		snprintf(config_path, sizeof(config_path), "%s/.config/cupidfetch/log.txt", home);
	}

	g_log = fopen(config_path, "w");
	if (g_log == NULL) {
	    g_log = stderr;
	    cupid_log(LogType_ERROR, "Couldn't open config, logging to stderr");
	}
    }
	
    if (parse_result == -69) {
        char config_path[CONFIG_PATH_SIZE];

	char *config = NULL;
	if ((config = getenv("XDG_CONFIG_HOME"))) {
		snprintf(config_path, sizeof(config_path), "%s/cupidfetch/cupidfetch.ini", config);
	} else {
		const char* home = get_home_directory();
		snprintf(config_path, sizeof(config_path), "%s/.config/cupidfetch/cupidfetch.ini", home);
	}

	if (access(config_path, F_OK) == -1)
		cupid_log(LogType_ERROR, "Couldn't open %s", config_path);
	else
		parse_result = ini_parse(config_path, cupid_ini_handler, &g_userConfig);

    }


    // It's fine not to stop the program, it will just run w/ the default
    // config
    if (parse_result < 0)
    	cupid_log(LogType_WARNING, "Running with default config");

    display_fetch();

    epitaph();
    return 0;
}

void epitaph() {
    if (g_log != stderr) fclose(g_log);
}
