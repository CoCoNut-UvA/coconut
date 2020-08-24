#include <stdlib.h>
#include <stdio.h>

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/set.h"
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

    set_st *set = SETnew_String(10);
    SETinsert(set, "Hello");
    bool yes = SETisMember(set, "Hello");
    bool no = SETisMember(set, "hello");
    SETremove(set, "Hello");
    DBUG_ASSERT(!SETisMember(set, "Hello"), "SHould not be a member!");
    printf("Yes:%d, No:%d\n", yes, no);

    SETinsert(set, "Hello");
    SETinsert(set, "Doei");

    set_st *copied = SETcpy(set);
    SETprint_String(copied);
    SETprint_String(set);

    set_st *set2 = SETnew_String(10);
    SETinsert(set2, "Doei");
    set_st *new = SETunion(set, set2);
    set_st *new2 = SETintersect(set, set2);
    set_st *diff = SETdifference(set, set2);

    SETprint_String(new);
    SETprint_String(new2);
    SETprint_String(diff);

    MEMfree(up);
    MEMfree(low);

    MEMmalloc((size_t)-1);
    CTIabortOnError();
    return 0;
}