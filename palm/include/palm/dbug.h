#pragma once

extern void DBUGprintAssert(int line, char *file, const char *func, char *msg, ...);

#define DBUG_ASSERT(expr, msg) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, msg);                \
        } \
    } while(0)


#define DBUG_ASSERTF(expr, format, args...) \
    do {                       \
        if (!(expr)) {         \
            DBUGprintAssert(__LINE__, __FILE__, __func__, format, args);                \
        } \
    } while(0)

