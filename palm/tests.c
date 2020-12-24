#include <stdlib.h>
#include <stdio.h>

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/dbug.h"

int main() {
    CTIerror("Hello");
    struct ctinfo info = {1, 1, 0, 0, "simple.ccn", "Phase"};
    CTIerrorObj(&info, "Phase unkown this is a rather long error message how is that handled i dont really know do you?");
    info.last_column = 5;
    CTIerrorObj(&info, "Phase unkown");
    CTIwarn("This is a warning");
    CTInote("You can find the phase here");
    CTIstate("Phase Root");

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
    CTIabortOnError();
    return 0;
}