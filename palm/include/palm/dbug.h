#pragma once

/**
 * @file
 * This file provides a small debug library.
 */

#include <stdio.h>

extern void DBUGprintAssert(int line, char *file, const char *func, char *msg, ...);
extern void DBUGprint(char *header, char *msg, ...);
extern void DBUGoff();

#ifndef NDEBUG
/**
 * When defined, will be used as a header before messages printed with DBUG calls.
 * Need to be defined before including this header.
 * DO NOT define MODULE inside a header file.
 */
#ifndef MODULE
#define MODULE NULL
#endif

/**
 * @brief Assert expression and print message when expressions fails.
 *        In most cases, a simple assert is good enough and you dont need this!
 * 
 * Example usage:
 *   - DBUG_ASSERT(true, "Hello DBUG");
 *   - DBUG_ASSERT(2 > 3, "%d is not bigger than %d", 2, 3); 
 * @param expr the expr to test, must be testable in an if.
 * @param ...  Variable args like printf, however, this contains the format string for portability.
 */
 #define DBUG_ASSERT(expr, ...) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, __VA_ARGS__);                \
        } \
    } while(0)


/**
 * Assert with format string.
 * DEPRECATED (just use DBUG_ASSERT).
 */
#define DBUG_ASSERTF(expr, format, ...) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, format, __VA_ARGS__);                \
        } \
    } while(0)

/**
 * @brief print a dbug message with the module, when defined, prepended.
 *
 * Example usage:
 *     - DBUG("Starting with phase");
 *     - DBUG("5+5=%d", 5+5);
 * @param ... Variable arguments. First message must be a format string.
 */
#define DBUG(...) \
    do { \
        DBUGprint(MODULE, __VA_ARGS__);\
    } while (0)

/**
 * Formatted debug message identified by header.
 * DEPRECATED (just use DBUG)
 */
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
