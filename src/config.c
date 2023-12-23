#include "cupidfetch.h"

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
    fprintf(config_file, "memory_unit = %s\n", default_config->memory_unit);
    fprintf(config_file, "memory_unit_size = %lu\n", default_config->memory_unit_size);

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
        } else if (strcmp(name, "memory_unit") == 0) {
	    if (value) strncpy(config->memory_unit, value, MEMORY_UNIT_LEN);
        } else if (strcmp(name, "memory_unit_size") == 0) {
	    config->memory_unit_size = (value != NULL) ? atol(value) : 1024 * 1024;
        }
    }

    return 1;
}



