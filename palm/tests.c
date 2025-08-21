#include <stdlib.h>
#include <stdio.h>

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/dbug.h"

int strncat_test()
{
    char first[7] = "Hello \0";
    char second[7] = "World!\0";

    char *result = STRncat(first, second, STRlen(second));
    printf("%s\n", result);

    MEMfree(result);
    
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

    //MEMmalloc((size_t)-1);


    strncat_test();

    return 0;
}