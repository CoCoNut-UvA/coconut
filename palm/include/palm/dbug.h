#pragma once

#include <stdio.h>

extern void DBUGprintAssert(int line, char *file, const char *func, char *msg, ...);
extern void DBUGprint(char *header, char *msg, ...);

#ifndef NDEBUG
// Assert and print message on fail.
#ifndef MODULE
#define MODULE NULL
#endif

// If you do not need a message, just use assert.
 #define DBUG_ASSERT(expr, msg, ...) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, msg, ##__VA_ARGS__);                \
        } \
    } while(0)


// Assert with format string.
// DEPRECATED (just use DBUG_ASSERT).
#define DBUG_ASSERTF(expr, format, ...) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, format, __VA_ARGS__);                \
        } \
    } while(0)

// Debug message identified by header.
#define DBUG(msg, ...) \
    do { \
        DBUGprint(MODULE, msg, ##__VA_ARGS__);\
    } while (0)

// Formatted debug message identified by header.
// DEPRECATED (just use DBUG)
#define DBUGF(header, format, ...) \
    do {                           \
        fprintf(stderr, "[%s] ", header); \
        fprintf(stderr, format, __VA_ARGS__);\
    } while (0)

#else
#define DBUG_ASSERT(expr, msg)


// Assert with format string.
#define DBUG_ASSERTF(expr, format, ...)

// Debug message identified by header.
#define DBUG(header, msg)

// Formatted debug message identified by header.
#define DBUGF(header, format, ...)
#endif
