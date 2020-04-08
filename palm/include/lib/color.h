#pragma once

#define DIM "\x1B[2m"
#define BOLD "\x1B[1m"

#define RED "\x1B[31m"
#define BRIGHT_RED BOLD RED
#define GREEN "\x1B[32m"
#define BRIGHT_GREEN BOLD GREEN
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define YELLOW "\x1B[33m"
#define BRIGHT_YELLOW BOLD YELLOW

#define RESET_COLOR "\x1B[0m"

#define PRINT_COLOR(c)                  \
    {                                   \
        if (isatty(STDERR_FILENO))      \
            fprintf(stderr, "%s", c);   \
    }

#define PRINT_COLOR_STDOUT(c)     \
    {                             \
	if (isatty(STDOUT_FILENO))    \
	    printf("%s", c);          \
    }
