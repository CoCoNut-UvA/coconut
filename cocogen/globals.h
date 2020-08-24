#pragma once

#include "palm/hash_table.h"
#include "format/format.h"

struct globals {
    char *filename;
    htable_st *line_map; // Maps line numbers to lines, this maps owns the lines values!
    struct format_settings *settings;
    char *gen_src_dir;
    char *gen_hdr_dir;
};

extern struct globals globals;