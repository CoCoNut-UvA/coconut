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

    OUT("#include <stddef.h>\n\n");
    OUT("#include \"ccngen/ccn_defs.h\"\n");
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("#include \"ccngen/enum.h\"\n");
    OUT("#ifdef CCN_USES_UNSAFE\n");
    OUT("#include \"user_types.h\"\n");
    OUT("#endif\n");
    OUT("typedef struct ccn_node %s;\n", "node_st");

    return root;
}

node_st *dynamicSwitchToAstSource(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "ast.c");

    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"palm/memory.h\"\n");
    OUT("#include \"palm/dbug.h\"\n");

    return root;
}

node_st *dynamic_start_equation_header(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "equation.h");

    return root;
}

node_st *dynamic_start_visit_header(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "visit.h");

    OUT("#include \"ccngen/ccn_defs.h\"\n");
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("#include \"ccngen/enum.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#ifdef CCN_USES_UNSAFE\n");
    OUT("#include \"user_types.h\"\n");
    OUT("#endif\n");

    return root;
}

node_st *dynamicSwitchToVisitSource(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "visit.c");

    OUT("#include \"ccngen/equation.h\"\n");
    OUT("#include \"ccngen/visit.h\"\n");
    OUT("#include \"palm/dbug.h\"\n\n");
    // Consider switching to C23 [[maybe_unused]] in the future
    OUT("#ifdef __GNUC__\n");
    OUT("#define MAYBE_UNUSED __attribute__((unused))\n");
    OUT("#else\n");
    OUT("#define MAYBE_UNUSED\n");
    OUT("#endif\n");

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

    OUT("#include \"ccngen/ccn_defs.h\"\n");
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

node_st *dynamic_start_ag_header(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "ag.h");
    OUT("#include \"ccn/ccn_types.h\"\n\n");

    return root;
}



node_st *dynamicSwitchToAGSource(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "ag.c");
    OUT("#include \"ccn/phase_driver.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"ccngen/ag.h\"\n");
    OUT("#include \"ccngen/enum.h\"\n");
    OUT("#include \"ccngen/visit.h\"\n");
    OUT("#include \"palm/ctinfo.h\"\n\n");
    return root;
}
