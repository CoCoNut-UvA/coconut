#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * Copy string and allocate memory for new string.
 *
 * @param source String to copy
 * @return New copied string
*/
extern char *STRcpy(const char *source);

/**
 * Copy string and allocate memory for new string.
 *
 * Copy only maxlen characters.
 *
 * @param source String to copy
 * @param maxlen Number of characters to copy
 * @return New copied string
*/
extern char *STRncpy(const char *source, int maxlen);

/**
 * Compare two strings.
 *
 * @param first First string to compare
 * @param second Second string to compare
 * @return True if string contents are equal, false if they are not equal.
*/
extern bool STReq(const char *first, const char* second);

/**
 * Compare two strings in a case insensitive way.
 *
 * @param first First string to compare
 * @param second Second string to compare
 * @return True if string contents are equal, false if they are not equal.
*/
extern bool STReqci(const char *first, const char* second);


/**
 * Compare two strings up to a certain limit of characters.
 *
 * @param first First string to compare
 * @param second Second string to compare
 * @param n number of relevant characters
 * @return True if the relevant prefixes of strings are equal,
 * false if they are not equal.
*/
extern bool STReqn(const char *first, const char* second, int n);

/**
 * Yield the length of a string
 *
 * Mostly to provide a complete interface and to avoid using standard
 * string facilities throughout the compiler.
 *
 * @return Length of the string
*/
extern size_t STRlen(const char *str);

/**
 * Concatenate two strings and allocate memory for new string.
 *
 * Copy only maxlen characters.
 *
 * @param first First string
 * @param second Second string
 * @return New concatenated string
*/
extern char *STRcat(const char *first, const char* second);

/**
 * Concatenate two strings and allocate memory for new string.
 * 
 * Copy only len, but at most `STRlen(secons)` characters.
 * 
 * @param first First string
 * @param second Second string
 * @param len Number of characters to copy from the second string
 * @return New concatenated string
 */
extern char* STRncat(const char *first, const char *second, size_t len);

/**
 * Concatenate n strings and allocate memory for new string.
 *
 * @param n Number of strings
 * @param ... n amount of "const char *"-type strings
 * @return New concatenated string
*/
extern char *STRcatn(int n, ...);

/**
 * Create a format string of an arbitrary length.
 *
 * Can be used like printf, but returns a string instead of printing to the
 * terminal.
 *
 * Similar to s(n)printf, but STRfmt will allocate a new string of the
 * required length, saving you the hassle of determining the length of a format
 * string.
 *
 * @param fmt format string that supports the same conversion specifiers as
 * printf. See `man 3 printf`
 * @param ... format string arguments Arbitrary amount of arguments for format
 * string
 *
 * @return The newly allocated formatted string
*/
extern char *STRfmt(const char *fmt, ...);

/**
 * Yields either the argument string if it is not NULL or an empty constant
 * string otherwise.
 *
 * This is helpful for instance when printing strings with the %s conversion
 * specifier and the string to print may be NULL.
 *
 * @param alt Alternative string to return if str is NULL
 * @param str String that may be NULL
 * @return str if `str != NULL`, else alt
*/
extern char *STRonNull( char *alt,  char *str);

/**
 * Convert integer to string in decimal representation.
 * This is equivalent to `STRfmt("%d", number)`.
 *
 * @param number to convert
 *
 * @return The newly allocated string representation of number
*/
extern char *STRitoa(int number);

/**
 * Convert a string to its lower form, but does not consume original string.
 * @param str The string to transform.
 * @return Allocated string, which is the lower form of str.
 */
extern char *STRlower(const char *str);

/**
 * Convert a string to its upper form, but does not consume original string.
 * @param str The string to transform.
 * @return Allocated string, which is the upper form of str.
 */
extern char *STRupper(const char *str);

/**
 * Tokenize string.
 *
 * On first call the str will be copied to internal static variable, next calls
 * str should be NULL. With last call the allocated memory of the copy will be
 * freed.
 *
 * In contrast to strtok, STRtok leaves the argument string untouched and always
 * allocates the tokens in fresh memory.
 *
 * @param str String to tokenize
 * @param tok Tokenizer
 * @return Either a pointer to the next token or NULL when no more tokens.
*/
extern char *STRtok(const char *str, const char *tok);

/**
 * Checks if prefix is at the start of str
 *
 * @param prefix First string to compare
 * @param str Second string to compare
 * @return True if prefix is at the start of str, false otherwise.
*/
extern bool STRprefix( const char *prefix, const char *str);

/**
 * Checks if suffix is at the end of str
 *
 * @param prefix First string to compare
 * @param str Second string to compare
 * @return True if suffix is at the end of str, false otherwise.
*/
extern bool STRsuffix( const char *suffix, const char *str);

/**
 * Checks if sub is contained inside of str
 *
 * @param sub Substring to find
 * @param str String to search
 * @return True when sub is a substring of str, false otherwise
*/
extern bool STRsub(const char *sub, const char *str);


/**
 * copy part of a string from start to start + len.
 *
 * If len is <0 then len is relative to the length of the string.
 *
 * @param string String to copy from
 * @param start Position to start copy
 * @param len Length to copy, can be negative
 * @return Copied part of the string
*/
extern char *STRsubStr( const char *string, int start, int len);


/**
 * Substitute all occurrences of token in str with subst
 *
 * @param str The string in which to make substitutions
 * @param token a string to substitute occurrences of in str
 * @param subst a string to substitute tokens with
 *
 * @return A new String or NULL if str is NULL
*/
extern char *STRsubstToken( const char *str, const char *token, const char *subst);


/**
 * Convert string to lower string by consuming the string.
 * @param str the string to convert to lower form.
*/
extern void STRtoLower(char *str);

/**
 * Convert string to upper string by consuming the string.
 * @param str the string to convert to upper form.
*/
extern void STRtoUpper(char *str);

/**
 * Yield an empty string.
 *
 * @return An allocated empty string
*/
extern char *STRnull( );

#define F_PTR "%p"
