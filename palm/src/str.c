
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "palm/str.h"

#include "palm/dbug.h"
#include "palm/memory.h"

/*******************************************************************************
 *
 * Description: Copy string and allocate memory for new string.
 *
 * Parameters: - source, string to copy
 *
 * Return: - new copied string
 *
 *******************************************************************************/

char *STRcpy(const char *source)
{
    char *ret;

    if (source != NULL) {
        ret = (char *)MEMmalloc(sizeof(char) * (STRlen(source) + 1));
        strcpy(ret, source);
    } else {
        ret = NULL;
    }

    return ret;
}

/*******************************************************************************
 *
 * Description: Copy string and allocate memory for new string.
 *              Copy only maxlen characters.
 *
 * Parameters: - source, string to copy
 *             - maxlen, number of characters to copy
 *
 * Return: - new copied string
 *
 *******************************************************************************/

char *STRncpy(const char *source, int maxlen)
{
    char *ret;
    int max;

    if (source != NULL) {
        max = STRlen(source);
        if (max > maxlen) {
            max = maxlen;
        }

        ret = (char *)MEMmalloc(sizeof(char) * (max + 1));
        strncpy(ret, source, max);

        /* make sure string ends with 0 */
        ret[max] = '\0';
    } else {
        ret = NULL;
    }

    return ret;
}

/** <!--********************************************************************-->
 *
 * @fn char *STRsubStr( const char *string, int start, int len)
 *
 * @brief copy part of a string from start to start + len.
 *        if len is <0 then len is relative to the length of the string.
 *
 *****************************************************************************/
char *STRsubStr(const char *string, int start, int len)
{
    int strlen = 0;
    char *ret = NULL;

    strlen = STRlen(string);

    if (len < 0) {
        len = strlen + len; /* + - => - */
    }

    if ((start + len) > strlen) { /* to long take what we can */
        len = strlen - start;
    }

    if (start > strlen) {
        ret = STRnull();
    } else {
        ret = memcpy(MEMmalloc(sizeof(char) * (len + 1)),
            string + start, /* move to start of sub string */
            len);
        ret[len] = '\0';
    }

    return ret;
}

/** <!--********************************************************************-->
 *
 * @fn char *STRnull( )
 *
 * @brief return an allocated empty string
 *
 *****************************************************************************/
char *STRnull()
{
    char *ret = NULL;

    ret = MEMmalloc(sizeof(char) * 1);
    ret[0] = '\0';

    return ret;
}

/*******************************************************************************
 *
 * Description: Concatenate two strings and allocate memory for new string.
 *
 * Parameters: - first, first string
 *             - second, second string
 *
 * Return: - new concatenated string
 *
 *******************************************************************************/

char *STRcat(const char *first, const char *second)
{
    char *result;

    if (first == NULL) {
        result = STRcpy(second);
    } else if (second == NULL) {
        result = STRcpy(first);
    } else {
        result = (char *)MEMmalloc(STRlen(first) + STRlen(second) + 1);

        strcpy(result, first);
        strcat(result, second);
    }

    return result;
}

/*******************************************************************************
 *
 * Description: Concatenate N strings and allocate memory for new string.
 *
 * Parameters: - n, number of strings
 *             - ..., n amount of "const char *"-type strings
 *
 * Return: - new concatenated string
 *
 *******************************************************************************/

char *STRcatn(int n, ...)
{
    int i;
    int length;
    char *result;
    const char *ptr;
    va_list arg_list;

    DBUG_ASSERTF(n > 2, "STRcatn called with %d arguments", n);

    va_start(arg_list, n);

    length = 0;

    for (i = 0; i < n; ++i) {
        ptr = va_arg(arg_list, const char *);
        if (ptr != NULL) {
            length += STRlen(ptr);
        }
    }

    va_end(arg_list);

    if (length == 0) {
        result = NULL;
    } else {
        result = (char *)MEMmalloc(length + 1);
        result[0] = '\0';

        va_start(arg_list, n);

        for (i = 0; i < n; ++i) {
            ptr = va_arg(arg_list, const char *);
            if (ptr != NULL) {
                strcat(result, ptr);
            }
        }

        va_end(arg_list);
    }

    return result;
}

/*******************************************************************************
 *
 * Description: Concatenate 2 strings.
 *              Only append n characters from the second string to the first one.
 *              If len is greater then STRlen(second), STRlen(second) will be used
 *              as len.
 *
 * Parameters: - first, first string to use
 *             - second, second string to use - append n characters to the first one
 *             - len, number of characters to copy from the second string              
 *
 * Return: - new concatenated string
 *
 *******************************************************************************/

 char* STRncat(const char *first, const char *second, size_t len)
 {
    char *result;
    size_t first_len = STRlen(first);
    size_t second_len = STRlen(second);
    if (len < second_len) { second_len = len; }

    if (first == NULL && second == NULL) {
        result = NULL;
    } else if (first == NULL && second != NULL) {
        result = STRncpy(second, second_len);
    } else if (first != NULL && second == NULL) {
        result = STRncpy(first, first_len);
    } else {
        size_t total_len = first_len + second_len + 1;
        result = MEMmalloc(first_len + second_len + 1);

        strncpy(result, first, first_len + 1);
        strncat(result, second, second_len);
    }

    return result;
 }

/*******************************************************************************
 *
 * Description: Compare two strings.
 *
 * Parameters: - first, first string to compare
 *             - second, second string to compare
 *
 * Return: - true, string contents are equal
 *         - false, string contents are not equal
 *
 *******************************************************************************/

bool STReq(const char *first, const char *second)
{
    bool res;

    if ((first == NULL) && (second == NULL)) {
        res = true;
    } else if ((first == NULL) || (second == NULL)) {
        res = false;
    } else {
        res = (0 == strcmp(first, second));
    }

    return res;
}

/*******************************************************************************
 *
 * Description: Compare two strings in a case insensitive way.
 *
 * Parameters: - first, first string to compare
 *             - second, second string to compare
 *
 * Return: - true, string contents are equal
 *         - false, string contents are not equal
 *
 *******************************************************************************/

bool STReqci(const char *first, const char *second)
{
    bool res;
    int i;

    if ((first == NULL) && (second == NULL)) {
        res = true;
    } else if ((first == NULL) || (second == NULL)) {
        res = false;
    } else {
        i = 0;
        while ((first[i] != '\0') && (second[i] != '\0')
            && (tolower(first[i]) == tolower(second[i]))) {
            i += 1;
        }
        if ((first[i] == '\0') && (second[i] == '\0')) {
            res = true;
        } else {
            res = false;
        }
    }

    return res;
}

/*******************************************************************************
 *
 * @brief Compare two strings up to a certain limit of characters.
 *
 * @param first, first string to compare
 * @param second, second string to compare
 * @param n, number of relevant characters
 *
 * @return: true when the relevant prefixes of strings are equal, false otherwise.
 *******************************************************************************/

bool STReqn(const char *first, const char *second, int n)
{
    bool res;

    if ((first == NULL) && (second == NULL)) {
        res = true;
    } else if ((first == NULL) || (second == NULL)) {
        if (n == 0) {
            res = true;
        } else {
            res = false;
        }
    } else {
        res = (0 == strncmp(first, second, n));
    }

    return res;
}

/*******************************************************************************
 *
 * @brief Checks if prefix is prefix of str
 *
 * @param prefix, first string to compare
 * @param str, second string to compare
 *
 * Return: - true, prefix is prefix of str
 *         - false, otherwise
 *
 *******************************************************************************/

bool STRprefix(const char *prefix, const char *str)
{
    bool res;

    if (prefix == NULL) {
        res = true;
    } else {
        if (str == NULL) {
            res = false;
        } else {
            int plen = STRlen(prefix);

            if (STRlen(str) < plen) {
                res = false;
            } else {
                res = (0 == strncmp(prefix, str, STRlen(prefix)));
            }
        }
    }

    return res;
}

/** <!--********************************************************************-->
 *
 * @fn bool *STRsuffix( const char *suffix, const char *str)
 *
 * @brief return true if suffix is the end of str, else return false.
 *
 *****************************************************************************/
bool STRsuffix(const char *suffix, const char *str)
{
    bool res = false;

    if (STRlen(suffix) > STRlen(str)) {
        res = false;
    } else {
        str = str + STRlen(str) - STRlen(suffix);
        res = (0 == strcmp(str, suffix));
    }

    return res;
}

/*******************************************************************************
 *
 * @brief Checks if sub is contained inside of str
 *
 * @param sub substring to find
 * @param str string to search
 *
 * @return true when sub is a substring of str, false otherwise
 *******************************************************************************/

bool STRsub(const char *sub, const char *str)
{
    bool res;

    if (sub == NULL) {
        res = true;
    } else {
        if (str == NULL) {
            res = false;
        } else {
            res = (NULL != strstr(str, sub));
        }
    }

    return res;
}

/*******************************************************************************
 *
 * @brief Yield length of string
 *
 *  Mostly to provide a complete interface and to avoid using standard
 *  string facilities throughout the compiler.
 *
 *******************************************************************************/

size_t STRlen(const char *s)
{
    size_t len;

    if (s == NULL) {
        len = 0;
    } else {
        len = strlen(s);
    }

    return len;
}


static bool CharInString(char c, const char *str)
{
    int i;
    bool res;

    if ((str == NULL) || (c == '\0')) {
        res = false;
    } else {
        i = 0;
        while ((str[i] != '\0') && (str[i] != c)) {
            i += 1;
        }
        res = str[i] != '\0';
    }

    return res;
}


/*******************************************************************************
 *
 * @brief Tokenize string. On first call the str will be copied to internal
 *        static variable, next calls str should be NULL. With last call the
 *        allocated memory of the copy will be freed.
 *
 *              In contrast to strtok, STRtok leaves the argument string untouched
 *              and always allocates the tokens in fresh memory.
 *
 * @param str string to tokenize
 * @param tok tokenizer
 *
 * @return either a pointer to the next token or NULL when no more tokens.
 *******************************************************************************/
char *STRtok(const char *first, const char *sep)
{
    static char *keep_string = NULL;
    static char *current = NULL;
    char *ret;
    int i;

    if (first != NULL) {
        if (keep_string != NULL) {
            keep_string = MEMfree(keep_string);
        }
        keep_string = STRcpy(first);
        current = keep_string;
    }

    if (current == NULL) {
        ret = NULL;
    } else {
        i = 0;
        while ((current[i] != '\0') && !CharInString(current[i], sep)) {
            i += 1;
        }

        if (current[i] == '\0') {
            ret = STRcpy(current);
            current = NULL;
        } else {
            current[i] = '\0';
            ret = STRcpy(current);
            current += i + 1;
        }
    }

    return ret;
}

/*******************************************************************************
 *
 * @brief
 *  yields either the argument string if it is not NULL or an empty constant
 *  string otherwise.
 *
 * This is helpful for instance when printing strings with the %s conversion
 * specifier and the string to print may be NULL.
 *
 *******************************************************************************/

char *STRonNull(char *alt, char *str)
{
    char *res;

    if (str == NULL) {
        res = alt;
    } else {
        res = str;
    }

    return res;
}

/*******************************************************************************
 *
 * Description: Create format string of arbitrary length. Use like printf.
 *
 * @param fmt format string
 * @param ... format string arguments
 *
 *
 * @return new allocated formatted string
 *
 *******************************************************************************/

char *STRfmt(const char *fmt, ...)
{
    char *str;
    int num;
    va_list arg_list;
    va_start(arg_list, fmt);
    // vsnprintf passed with size 0 will not write any bytes, but returns
    // the desired length of the format string
    num = vsnprintf(NULL, 0, fmt, arg_list);
    va_end(arg_list);
    DBUG_ASSERT(num >= 0, "Trouble in STRfmt");

    // Allocate array large enough to store format
    str = (char *)MEMmalloc(sizeof(char) * (num + 1));
    va_start(arg_list, fmt);
    num = vsnprintf(str, num + 1, fmt, arg_list);
    va_end(arg_list);
    DBUG_ASSERT(num >= 0, "Trouble in STRfmt");
    return str;
}

/*******************************************************************************
 *
 * Description: Convert integer to string in decimal representation.
 * This is equivalent to `STRfmt("%d", number)`.
 *
 * @param number to convert
 *
 * @return new allocated string representation of number
 *
 *******************************************************************************/

char *STRitoa(int number)
{
    return STRfmt("%d", number);
}

/** <!-- ****************************************************************** -->
 * @brief Substitute all occurrences of token in str with subst
 *
 * @param str The string in which to make substitutions
 * @param token a string to substitute occurrences of in str
 * @param subst a string to substitute tokens with
 *
 * @return A new String or NULL if str is NULL
 ******************************************************************************/
char *STRsubstToken(const char *str, const char *token, const char *subst)
{
    int occurrences, tlen, slen;
    const char *found;
    char *pos;
    char *result;

    /* Find number of occurrences of token in str */
    occurrences = 0;
    tlen = STRlen(token);
    slen = STRlen(subst);
    found = strstr(str, token);

    while (found != NULL) {
        occurrences++;
        found = strstr(found + tlen, token);
    }

    /* Make substitutions */
    result = MEMmalloc((STRlen(str) + (occurrences * (STRlen(subst) - tlen)) + 1) * sizeof(char));

    pos = result;
    while (*str != '\0') {
        if (STRprefix(token, str)) {
            strncpy(pos, subst, slen);
            pos += slen;
            str += tlen;
        } else {
            *(pos++) = *(str++);
        }
    }
    *pos = '\0';

    return result;
}

/**
 * Convert string to lower string by consuming the string.
 * @param str the string to convert to lower form.
 */
void STRtoLower(char *str)
{
    if (!str) {
        return;
    }

    while (*str != '\0') {
        *str = tolower(*str);
        str++;
    }
}

/**
 * Convert string to upper string by consuming the string.
 * @param str the string to convert to upper form.
 */
void STRtoUpper(char *str)
{
    if (!str) {
        return;
    }

    while (*str != '\0') {
        *str = toupper(*str);
        str++;
    }
}

/**
 * Convert a string to its lower form, but does not consume original string.
 * @param str The string to transform.
 * @return Malloced string, which is the lower form of str.
 */
char *STRlower(const char *str)
{
    if (!str) {
        return NULL;
    }

    char *ret = MEMmalloc(sizeof(char) * (STRlen(str) + 1));
    char *pos = ret;

    while (*str != '\0') {
        *pos = tolower(*str);
        pos++;
        str++;
    }
    *pos = '\0';

    return ret;
}

/**
 * @brief Convert a string to its upper form, but does not consume the original string.
 * @param str The string to transform
 * @return Allocated string, which is the upper form of str.
 */
char *STRupper(const char *str)
{
    if (!str) {
        return NULL;
    }

    char *ret = MEMmalloc(sizeof(char) * (STRlen(str) + 1));
    char *pos = ret;

    while (*str != '\0') {
        *pos = toupper(*str);
        pos++;
        str++;
    }
    *pos = '\0';

    return ret;
}
