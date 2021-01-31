#pragma once

#include <stdio.h>

extern void DBUGprintAssert(int line, char *file, const char *func, char *msg, ...);

// Assert and print message on fail.
#define DBUG_ASSERT(expr, msg) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, msg);                \
        } \
    } while(0)


// Assert with format string.
#define DBUG_ASSERTF(expr, format, ...) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, format, __VA_ARGS__);                \
        } \
    } while(0)

// Debug message identified by header.
#define DBUG(header, msg) \
    do { \
        fprintf(stderr, "[%s] %s\n", header, msg); \
    } while (0)

// Formatted debug message identified by header.
#define DBUGF(header, format, ...) \
    do {                           \
        fprintf(stderr, "[%s] ", header); \
        fprintf(stderr, format, __VA_ARGS__);\
    } while (0)
