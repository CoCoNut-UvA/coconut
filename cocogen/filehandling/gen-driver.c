//
// Created by damian on 19-08-20.
//
#include "gen-driver.h"


#include "globals.h"
#include "palm/memory.h"
#include "palm/str.h"

FILE *FHgetIncludeFile(const char *filename)
{
    char *full_file = STRcat(globals.gen_hdr_dir, filename);
    printf("%s\n", full_file);
    FILE *fp = fopen(full_file, "w");
    MEMfree(full_file);

    return fp;
}

FILE *FHgetSourceFile(const char *filename)
{
    char *full_file = STRcat(globals.gen_src_dir, filename);
    FILE *fp = fopen(full_file, "w");
    MEMfree(full_file);

    return fp;
}