// Cupidfetch is a system information retrieval tool written in C for Linux systems. It's a beginner-friendly, work-in-progress hobby project aimed at learning and exploring programming concepts.

#ifndef CUPIDFETCH_H
#define CUPIDFETCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <pwd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdarg.h>
#include <dirent.h>
// Inih
#include <ini.h>

#define LINUX_PROC_LINE_SZ 128
#define MEMORY_UNIT_LEN 128

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
    int display_window_manager;
    int display_local_ip;
    int display_available_memory;
    char memory_unit[MEMORY_UNIT_LEN];
    unsigned long memory_unit_size;
    int display_available_storage;
    char storage_unit[MEMORY_UNIT_LEN];
    unsigned long storage_unit_size;
};

// print.c
int get_terminal_width();
void print_info(const char *key, const char *format, int align_key, int align_value, ...);
void print_cat(const char* distro);


// modules.c
void get_hostname();
void get_username();
void get_linux_kernel();
void get_uptime();
void get_package_count(const char* distro);
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

#endif
