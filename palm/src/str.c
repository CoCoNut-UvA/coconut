
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "lib/assert.h"
#include "lib/log.h"
#include "lib/str.h"
#include "lib/memory.h"
#include "lib/array.h"


char *ccn_str_cpy(const char *source) {
    ccn_contract_in(source != NULL);

    char *copy = strdup(source);
    if (copy == NULL) {
	ccn_log_error("Could not duplicate strings");
	exit(EXIT_FAILURE);
    }

    return copy;
}


char *ccn_str_cat(const char *first, const char *second) {
    ccn_contract_in(first != NULL);
    ccn_contract_in(second != NULL);

    size_t len_first = strlen(first);
    size_t len_second = strlen(second);

    char *result = mem_alloc(sizeof(char) * (len_first + len_second + 1));

    strcpy(result, first);
    strcat(result, second);
    return result;
}


char *ccn_str_cat_n(const int n, ...) {
    ccn_contract_in(n > 0);

    size_t size = 0;
    va_list va_args;
    va_start(va_args, n);

    for (int i = 0; i < n; i++) {
        char *val = va_arg(va_args, char*);
	ccn_contract_in(val != NULL);
        size += strlen(val);
    }
    va_end(va_args);

    char *result = (char*)mem_alloc(sizeof(char) * (size + 1));
    result[0] = '\0'; // We start with an empty string.

    if (size == 0) return result;

    va_start(va_args, n);

    for (int i = 0; i < n; i++) {
        char *val = va_arg(va_args, char*);
        strcat(result, val);
    }
    va_end(va_args);

    return result;
}


char *ccn_str_cat_array(const array *strings) {
    ccn_contract_in(strings != NULL);

    size_t size = 0;

    for (int i = 0; i < array_size(strings); ++i) {
        char *val = array_get(strings, i);
        assert(val != NULL);
        size += strlen(val);
    }

    char *result = (char*)mem_alloc(sizeof(char) * (size + 1));
    result[0] = '\0'; // We start with an empty string.

    for (int i = 0; i < array_size(strings); ++i) {
        char *val = array_get(strings, i);
        strcat(result, val);
    }

    return result;
}


bool ccn_str_equal(const char *first, const char *second) {
    if (first == second) return true;
    return strcmp(first, second) == 0;
}


array *ccn_str_split(char *target, const char delimeter) {
    array *matches = create_array();
    char *start = target;
    char *current = target;
    size_t size = 0;

    while (*current) {
        if (*current == delimeter) {
            char *match = mem_copy(start, size + 1);
            match[size] = '\0';
            array_append(matches, match);
            size = 0;
            start = current + 1;
        } else {
            size++;
        }
        current++;
    }

    char *last_match = mem_copy(start, size + 1);
    last_match[size] = '\0';
    array_append(matches, last_match);

    return matches;
}


bool ccn_str_startswith(const char *str, const char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}
