
#include "gen_helpers/out_macros.h"
#include "gen_files.h"
#include "palm/memory.h"
#include "palm/str.h"
#include "globals.h"

FILE *FSgetIncludeFile(const char *filename)
{
    char *full_file = STRcat(globals.gen_hdr_dir, filename);
    FILE *fp = fopen(full_file, "w");
    if (!fp) {
        fprintf(stderr, "Could not open file: %s\n", full_file);
        exit(1);
    }
    MEMfree(full_file);
    {
        int indent = 0;
        OUT("#pragma once\n");
    }
    return fp;
}

FILE *FSgetSourceFile(const char *filename)
{
    char *full_file = STRcat(globals.gen_src_dir, filename);
    FILE *fp = fopen(full_file, "w");
    if (!fp) {
        fprintf(stderr, "Could not open file: %s\n", full_file);
        exit(1);
    }
    MEMfree(full_file);

    return fp;
}
