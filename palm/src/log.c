#include "lib/log.h"
#include "lib/color.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

void _ccn_log_handler(enum _logTypeCCN log_type, const char *file, const char *func, unsigned int line, const char *format, ...)
{
    char *color = "";
    char *header = "";

    switch(log_type) {
    case InfoCCNlogType:
        color = GREEN;
        header = "INFO";
        break;
    case WarnCCNlogType:
        color = BRIGHT_YELLOW;
        header = "WARN";
        break;
    case DebugCCNlogType:
        color = BRIGHT_GREEN;
        header = "DEBUG";
        break;
    case ErrorCCNlogType:
        color = RED;
        header = "ERROR";
        break;
    case FatalCCNlogType:
        color = BRIGHT_RED;
        header = "FATAL";
        break;
    }

    PRINT_COLOR(color);
    fprintf(stderr, "%s: ", header);
    PRINT_COLOR(RESET_COLOR);
    PRINT_COLOR(DIM);
    fprintf(stderr, "%s:%d:%s: ", file, line, func);
    PRINT_COLOR(RESET_COLOR);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    fputc('\n', stderr);
    va_end(args);

    if (log_type == FatalCCNlogType) abort();
}
