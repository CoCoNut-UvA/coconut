#include <stdlib.h>
#include <stdio.h>

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/dbug.h"

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

    MEMmalloc((size_t)-1);
    return 0;
}