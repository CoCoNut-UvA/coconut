#include <gen_helpers/out_macros.h>
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "globals.h"
#include "ccn/dynamic_core.h"

node_st *GGHast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "gate.h");

    TRAViphases(node);
    return node;
}

node_st *GGHiphase(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (IPHASE_GATE_FUNC(node)) {
        OUT("bool %s();\n", ID_ORIG(IPHASE_GATE_FUNC(node)));
    }
    TRAVnext(node);
    return node;
}
