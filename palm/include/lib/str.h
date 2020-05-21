#pragma once

#include "array.h"
#include <stdbool.h>

extern char *ccn_str_cpy(const char *source);
extern char *ccn_str_lwr(const char *source);
extern char *ccn_str_upr(const char *source);
extern char *ccn_str_cat(const char *first, const char *second);
extern bool ccn_str_equal(const char *first, const char *second);

/* Concatenate $n$ strings together.
 * In case of all empty string the return string will be empty as well.
 *
 * CONTRACT:
 *  - none of the values may be NULL, in case of NULL it is UB,
 *  - n > 0.
 */
extern char *ccn_str_cat_n(const int n, ...);

/* Concatenate an array of strings into one string.
 * When all strings are empty, the return value is an empty string as well.
 *
 * CONTRACT:
 *  - none of the strings may be NULL, in case it is UB,
 *  - the array must not be NULL, in case it is UB,
 *  - all data in the array is of type char*, otherwise UB.
 */
extern char *ccn_str_cat_array(const array *strings);

/* Splits a string on the given delimeter. Returns an allocated array of all
 * the sub-strings that where created. If the delimiter was not found
 * the sub-string will be the same as the target string.
 *
 * CONTRACT:
 *  - The array can not be NULL.
 *
 * NOTE: do not forget to free the array and the strings in the array.
 */
extern array *ccn_str_split(char *target, const char delimeter);

bool ccn_str_startswith(const char *str, const char *pre);
