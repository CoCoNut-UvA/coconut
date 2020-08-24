#include "palm/dbug.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void DBUGprintAssert(int line, char *file, const char *func, char *msg, ...) {
    va_list arg_p;

    va_start(arg_p, msg);

    fprintf(stderr, "ASSERTION FAILED: file %s, line %d in function: %s\n", file, line, func);
    fprintf(stderr, "\t%s\n", msg);
    fprintf(stderr, "ABORTING EXECUTION");

    va_end(arg_p);

    abort();
}
