#include <stdlib.h>
#include <stdio.h>

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/dbug.h"

#include <string.h>


void strncat_test2()
{
    char second[2] = "i\0";
    char *result = STRncat("__for__", second, STRlen(second));

    DBUG_ASSERT(
        strncmp(result, "__for__i\0", strlen(result)) == 0,
        "expected \"__for__i\", got %s",
        result
    );

    printf("%s\n", result);

    MEMfree(result);
}

void strncat_test1()
{
    char first[7] = "Hello \0";
    char second[7] = "World!\0";

    char *result = STRncat(first, second, STRlen(second));
    DBUG_ASSERT(
        strncmp(result, "Hello World!\0", strlen(result)) == 0,
        "expected \"Hello World!\", got %s",
        result
    );

    printf("%s\n", result);

    MEMfree(result);
}

void strncat_test()
{
    strncat_test1();
    strncat_test2();
}

int main() {
    struct ctinfo info = {1, 1, 0, 0, "simple.ccn", "Phase"};
    info.last_column = 5;

    char *orig = "hello";
    char *up = STRupper(orig);
    char *low = STRlower(up);
    char *none = STRlower(NULL);
    none = STRupper(none);

    printf("%s:%s:%s\n", orig, up, low);
    printf("%s\n", STRonNull("NULL :)", none));

    MEMfree(up);
    MEMfree(low);

    strncat_test();

    MEMmalloc((size_t)-1);

    return 0;
}