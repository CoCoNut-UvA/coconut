#pragma once

extern enum {
    SUCCESS = 0,
    CANNOT_CREATE_DIR = 1,
    CANNOT_OPEN_FILE = 2,
    HASH_ERROR = 3,
    INVALID_CONFIG = 4,
    MALLOC_NULL = 5,
    REGEX_COMPILE_ERROR = 6,
    UNKNOWN_BASICTYPE = 7,
    INVALID_BACKEND = 8,
} ErrorCode;
