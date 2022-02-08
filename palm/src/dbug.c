#include "palm/dbug.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

static bool debug = true;

void DBUGprintAssert(int line, char *file, const char *func, char *msg, ...)
{
    va_list arg_p;

    va_start(arg_p, msg);

    fprintf(stderr, "ASSERTION FAILED: file %s, line %d in function: %s\n", file, line, func);
    fprintf(stderr, "\t%s\n", msg);
    fprintf(stderr, "ABORTING EXECUTION");

    va_end(arg_p);

    abort();
}

void DBUGprint(char *header, char *msg, ...)
{
    if (!debug) {
        return;
    }
    va_list arg_p;

    va_start(arg_p, msg);

    if (header) {
        printf("[%s] ", header);
    }
    vprintf(msg, arg_p);

    va_end(arg_p);
}

/**
 * Turn off debugging. Doesn't print anything!
 */
extern void DBUGoff()
{
    debug = false;
}
