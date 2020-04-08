#pragma once

enum _logTypeCCN {
    InfoCCNlogType,
    DebugCCNlogType,
    FatalCCNlogType,
    WarnCCNlogType,
    ErrorCCNlogType
};

extern void _ccn_log_handler(enum _logTypeCCN type, const char *file, const char *func, unsigned int line, const char *format, ...);

// TODO: Make this based on some log level? Debug probably by NDEBUG
// Error always present and fatal?
#ifndef NDEBUG
#define ccn_log_info(...) (_ccn_log_handler(InfoCCNlogType, __FILE__, __func__, __LINE__, __VA_ARGS__))
#define ccn_log_debug(...) (_ccn_log_handler(DebugCCNlogType, __FILE__, __func__, __LINE__, __VA_ARGS__))
#define ccn_log_fatal(...) (_ccn_log_handler(FatalCCNlogType, __FILE__, __func__, __LINE__, __VA_ARGS__))
#define ccn_log_warn(...) (_ccn_log_handler(WarnCCNlogType, __FILE__, __func__, __LINE__, __VA_ARGS__))
#define ccn_log_error(...) (_ccn_log_handler(ErrorCCNlogType, __FILE__, __func__, __LINE__, __VA_ARGS__))
#else
#define ccn_log_info(...) ((void)0)
#define ccn_log_debug(...) ((void)0) 
#define ccn_log_fatal(...) ((void)0)
#define ccn_log_warn(...) ((void)0)
#define ccn_log_error(...) ((void)0)
#endif

