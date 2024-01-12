#include "cupidfetch.h"

const char *log_types[] = {"INFO", "WARNING", "ERROR", "CRITICAL"};

void cupid_log(LogType ltp, const char *format, ...) {
    va_list args;
    va_start(args, format);

    fprintf(g_log, "%s: <", log_types[ltp]);
    vfprintf(g_log, format, args);
    fprintf(g_log, ">\n");

    va_end(args);

    if (ltp == LogType_CRITICAL) {
	epitaph();
	exit(EXIT_FAILURE);
    }
}

