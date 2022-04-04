#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "globals.h"
#include "palm/str.h"
#include "palm/memory.h"
#include "gen_helpers/out_macros.h"
#include "dynamic_backend/gen_helpers.h"
#include <err.h>

node_st *DUGPast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenUserFile(ctx, "passes.c");
    OUT("#include \"ccn/ccn.h\"\n\n");
    OUT("#include \"ccngen/ast.h\"\n\n");
    TRAVopt(AST_IPASSES(node));
    return node;
}

node_st *DUGPipass(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *func = DGHpassFuncName(node);
    OUT("node_st *%s(node_st *root)\n{\n    return root;\n}\n\n", func);
    MEMfree(func);
    TRAVopt(IPASS_NEXT(node));
    return node;
}
