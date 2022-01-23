#pragma once

#include "palm/hash_table.h"
#include <stdio.h>
#include "generator/generator.h"

struct globals {
    char *filename;
    htable_st *line_map; // Maps line numbers to lines, this maps owns the lines values!
    char *gen_src_dir;
    char *gen_hdr_dir;
    char *gen_user_dir;
    char *backend;
    bool show_ast;
    GeneratorContext *gen_ctx;
};

extern struct globals globals;
