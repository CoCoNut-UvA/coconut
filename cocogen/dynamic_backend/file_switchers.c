/**
 * @file
 *
 * Contains most passes that handle creation and closing of file handlers
 * used in traversals.
 */

#include "stdio.h"
#include "gen_helpers/out_macros.h"
#include "globals.h"
#include <stdio.h>

node_st *dynamic_start_ast_header(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "ast.h");

    OUT("#pragma once\n");
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("#include \"ccngen/enum.h\"\n");
    OUT("typedef struct ccn_node %s;\n", "node_st");

    return root;
}

node_st *dynamicSwitchToAstSource(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "ast.c");

    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"palm/memory.h\"\n");

    return root;
}

node_st *dynamicStartEnumHeader(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "enum.h");
    return root;
}

node_st *dynamicStartTableGen(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "vtables.c");
    OUT("#include \"ccngen/actions.h\"\n");
    OUT("#include \"ccn/ccn_types.h\"\n");

    return root;
}

node_st *dynamicStartTravData(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "trav_data.h");

    OUT("#include\"ccngen/ccn_defs.h\"\n");
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("#ifdef CCN_USES_UNSAFE\n");
    OUT("#include \"user_types.h\"\n");
    OUT("#endif\n");
    OUT("#include \"ccngen/ast.h\"\n");

    return root;
}

node_st *dynamicSwitchTravData(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "trav_data.c");
    OUT("#include <stdlib.h>\n");
    OUT("#include \"ccngen/trav_data.h\"\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("#include \"palm/memory.h\"\n");


    return root;
}
