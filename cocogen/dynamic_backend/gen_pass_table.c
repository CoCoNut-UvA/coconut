
#include <stddef.h>
#include <stdio.h>
#include <globals.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "palm/memory.h"
#include "ccn/dynamic_core.h"
#include "dynamic_backend/gen_helpers.h"

static node_st *ast;

node_st *DGPTast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    ast = node;
    OUT("const ccn_pass_ft ccn_pass_vtable[_PASS_SIZE] = { &PASSerror, ");
    TRAVopt(AST_IPASSES(node));
    OUT("};\n");
    return node;
}


node_st *DGPTipass(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *func = DGHpassFuncName(node);
    OUT("&%s, ", func);
    MEMfree(func);
    TRAVopt(IPASS_NEXT(node));
    return node;
}
