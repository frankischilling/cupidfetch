#include "cupidfetch.h"


const char *log_types[3] = {"INFO", "WARNING", "ERROR"};

void cupid_log(LogType ltp, const char *format, ...) {
    va_list args;
    va_start(args, format);

    fprintf(g_log, "%s: <", log_types[ltp]);
    vfprintf(g_log, format, args);
    fprintf(g_log, ">\n");

    va_end(args);
}

