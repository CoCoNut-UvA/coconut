#include "stdio.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include "filesystem/gen_files.h"
#include <stdio.h>

node_st *dynamic_start_ast_header(node_st *root)
{
    FILE *fp = FSgetIncludeFile("ast.h");
    globals.fp = fp;
    int indent = 0;
    OUT("#pragma once\n");
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("#include \"ccngen/enum.h\"\n");
    OUT("typedef struct ccn_node %s;\n", "node_st");

    return root;
}

node_st *dynamic_switch_to_ast_source(node_st *root)
{
    fclose(globals.fp);
    globals.fp = FSgetSourceFile("ast.c");
    FILE *fp = globals.fp;
    int indent = 0;
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"palm/memory.h\"\n");

    return root;
}

node_st *dynamic_start_enum_header(node_st *root)
{
    fclose(globals.fp);
    globals.fp = FSgetIncludeFile("enum.h");
    FILE *fp = globals.fp;
    int indent = 0;
    OUT("#pragma once\n");
    return root;
}

node_st *dynamic_start_table_gen(node_st *root)
{
    fclose(globals.fp);
    globals.fp = FSgetSourceFile("vtables.c");
    FILE *fp = globals.fp;
    int indent = 0;
    OUT("#include \"ccngen/actions.h\"\n");
    OUT("#include \"ccn/ccn_types.h\"\n");

    return root;
}