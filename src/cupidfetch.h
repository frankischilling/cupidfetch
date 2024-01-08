// Cupidfetch is a system information retrieval tool written in C for Linux systems. It's a beginner-friendly, work-in-progress hobby project aimed at learning and exploring programming concepts.

#ifndef CUPIDFETCH_H
#define CUPIDFETCH_H

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <ifaddrs.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

// Inih
#include <ini.h>

#define MAX_NUM_MODULES 30
#define CONFIG_PATH_SIZE 256
#define LINUX_PROC_LINE_SZ 128
#define MEMORY_UNIT_LEN 128


struct CupidConfig {
    void (*modules[MAX_NUM_MODULES + 1])(void);
    char memory_unit[MEMORY_UNIT_LEN];
    unsigned long memory_unit_size;
    char storage_unit[MEMORY_UNIT_LEN];
    unsigned long storage_unit_size;
};

typedef enum {
	LogType_INFO = 0,
	LogType_WARNING = 1,
	LogType_ERROR = 2,
	LogType_CRITICAL = 3
} LogType;

// print.c
int get_terminal_width();
void print_info(const char *key, const char *format, int align_key, int align_value, ...);
void print_cat(const char* distro);


// modules.c
void get_hostname();
void get_username();
void get_linux_kernel();
void get_uptime();
void get_distro();
void get_package_count();
void get_shell();
void get_terminal();
void get_desktop_environment();
/* void get_window_manager(); */
void get_local_ip();
void get_available_memory();
void get_available_storage();
const char* get_home_directory();


// config.c
extern struct CupidConfig g_userConfig;
void init_g_config();
void create_default_config(const char* config_path, const struct CupidConfig* default_config);
int cupid_ini_handler(void* user, const char* section, const char* name, const char* value);

// log.c
void cupid_log(LogType ltp, const char *format, ...);

// main.c
extern FILE *g_log;
const char* detect_linux_distro();
void epitaph();


#endif
