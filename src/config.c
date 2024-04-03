#include "cupidfetch.h"

struct CupidConfig g_userConfig;

struct module {
    char *s;
    void (*m)();
};
struct module string_to_module[] = {
    {"hostname", get_hostname},
    {"username", get_username},
    {"distro", get_distro},
    {"linux_kernel", get_linux_kernel},
    {"kernel", get_linux_kernel},
    {"uptime", get_uptime},
    {"pkg", get_package_count},
    {"term", get_terminal},
    {"shell", get_shell},
    {"de", get_desktop_environment},
    /* TODO: window manager module reactivation */
    // {"window_manager", 
    {"ip", get_local_ip},
    {"memory", get_available_memory},
    {"storage", get_available_storage},
    {"cpu", get_cpu},
};

void init_g_config() {
    struct CupidConfig cfg_ = {
	.modules = { get_hostname, get_username, get_distro, get_linux_kernel
	           , get_uptime, get_package_count, get_terminal, get_shell
		   , get_desktop_environment, get_local_ip, get_available_memory, get_cpu
		   , get_available_storage
		   , NULL
		   },
        .memory_unit = "MB",
        .memory_unit_size = 1000000,
        .storage_unit = "GB",
        .storage_unit_size = 1000000000,
    };
    g_userConfig = cfg_;
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
    } else if (strcmp(section, "memory") == 0) {
        if (strcmp(name, "unit-str") == 0) {
	    if (value) strncpy(config->memory_unit, value, MEMORY_UNIT_LEN);
        } else if (strcmp(name, "unit-size") == 0) {
	    config->memory_unit_size = (value != NULL) ? atol(value) : 1024 * 1024;
	} else return 0;
    } else if (strcmp(section, "storage") == 0) {
        if (strcmp(name, "unit-str") == 0) {
	    if (value) strncpy(config->storage_unit, value, MEMORY_UNIT_LEN);
        } else if (strcmp(name, "unit-size") == 0) {
	    config->storage_unit_size = (value != NULL) ? atol(value) : 1024 * 1024;
        } else return 0;
    } else return 0;

    return 1;
}
