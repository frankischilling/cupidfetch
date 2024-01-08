#include "cupidfetch.h"

struct CupidConfig g_userConfig;


struct {
    char *s;
    void (*m)(void);
} string_to_module[] = {
    {"host_name", get_hostname},
    {"hostname", get_hostname},
    {"username", get_username},
    {"distro", get_distro},
    {"linux_kernel", get_linux_kernel},
    {"uptime", get_uptime},
    {"package_count", get_package_count},
    {"pkg", get_package_count},
    {"shell", get_shell},
    {"desktop_environment", get_desktop_environment},
    {"de", get_desktop_environment},
    // {"window_manager", 
    {"local_ip", get_local_ip},
    {"ip", get_local_ip},
    {"available_memory", get_available_memory},
    {"memory", get_available_memory},
    {"available_storage", get_available_storage},
    {"storage", get_available_storage}
};


void init_g_config() {
    struct CupidConfig cfg_ = {
	.modules = {NULL},
        .memory_unit = "MiB",
        .memory_unit_size = 1024 * 1024,
        .storage_unit = "GiB",
        .storage_unit_size = 1024 * 1024 * 1024,
    };
    g_userConfig = cfg_;
}


void create_default_config(const char* config_path, const struct CupidConfig* default_config) {
    FILE* config_file = fopen(config_path, "w");
    if (config_file == NULL) {
        fprintf(stderr, "Error creating config file: %s\n", config_path);
        exit(EXIT_FAILURE);
    }

    fprintf(config_file, "[DisplayOptions]\n");
    // FIXME: hardcoded
    fprintf(config_file, "modules = host_name username distro linux_kernel uptime package_count shell desktop_environment local_ip available_memory available_storage\n");
    fprintf(config_file, "memory_unit = %s\n", default_config->memory_unit);
    fprintf(config_file, "memory_unit_size = %lu\n", default_config->memory_unit_size);
    fprintf(config_file, "storage_unit = %s\n", default_config->storage_unit);
    fprintf(config_file, "storage_unit_size = %lu\n", default_config->storage_unit_size);

    fclose(config_file);
}

// INI handler function
int cupid_ini_handler(void* user, const char* section, const char* name, const char* value) {
    struct CupidConfig* config = (struct CupidConfig*)user;


    if (!section || section[0] == '\0') {
	if (strcmp(name, "modules") == 0) {
	    char v[INI_MAX_LINE];
	    strncpy(v, value, INI_MAX_LINE);
	    v[INI_MAX_LINE - 1] = '\0';

            char *module = strtok(v, " ");
	    size_t mi = 0;
	    while (module) {
	        printf("(%s)\n", module);

		for (size_t i = 0; i < sizeof(string_to_module) / sizeof(*string_to_module); i++) {
		    if (strcmp(module, string_to_module[i].s) == 0) {
		        if (mi + 1 >= MAX_NUM_MODULES) return 0;

			config->modules[mi] = string_to_module[i].m;
			config->modules[mi + 1] = NULL;
			mi++;
			
			break;
		    }
		}

		module = strtok(NULL, " ");
	    }
	} else {
	    return 0;
	}
    } else if (strcmp(section, "DisplayOptions") == 0) {
        if (strcmp(name, "memory_unit") == 0) {
	    if (value) strncpy(config->memory_unit, value, MEMORY_UNIT_LEN);
        } else if (strcmp(name, "memory_unit_size") == 0) {
	    config->memory_unit_size = (value != NULL) ? atol(value) : 1024 * 1024;
        } else if (strcmp(name, "storage_unit") == 0) {
	    if (value) strncpy(config->storage_unit, value, MEMORY_UNIT_LEN);
        } else if (strcmp(name, "storage_unit_size") == 0) {
	    config->storage_unit_size = (value != NULL) ? atol(value) : 1024 * 1024;
        } else {
            return 0;  /* unknown section/name, error */
        }

    }

    return 1;
}
