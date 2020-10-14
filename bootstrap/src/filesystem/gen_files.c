
#include "gen_files.h"
#include "palm/memory.h"
#include "palm/str.h"
#include "globals.h"

FILE *FSgetIncludeFile(const char *filename)
{
    char *full_file = STRcat(globals.gen_hdr_dir, filename);
    FILE *fp = fopen(full_file, "w");
    MEMfree(full_file);

    return fp;
}

FILE *FSgetSourceFile(const char *filename)
{
    char *full_file = STRcat(globals.gen_src_dir, filename);
    FILE *fp = fopen(full_file, "w");
    MEMfree(full_file);

    return fp;
}