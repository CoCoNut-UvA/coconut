#pragma once

#include <stdio.h>

extern void DBUGprintAssert(int line, char *file, const char *func, char *msg, ...);

#define DBUG_ASSERT(expr, msg) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, msg);                \
        } \
    } while(0)


#define DBUG_ASSERTF(expr, format, ...) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, format, __VA_ARGS__);                \
        } \
    } while(0)


#define DBUG(msg) \
    do { \
        printf("%s:%d| %s", __FILE__, __LINE__, msg); \
    } while (0)

#define DBUGF(format, ...) \
    do {                      \
        printf("%s:%d| ", __FILE__, __LINE__); \
        printf(format, __VA_ARGS__);\
    } while (0)
