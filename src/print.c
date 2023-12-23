#include "cupidfetch.h"

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