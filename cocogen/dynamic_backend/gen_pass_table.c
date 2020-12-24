
#include <stddef.h>
#include <stdio.h>
#include <globals.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

static FILE *fp;
static int indent = 0;
static node_st *ast;

node_st *DGPTast(node_st *node)
{
    fp = globals.fp;
    ast = node;
    OUT("const ccn_pass_ft ccn_pass_vtable[_PASS_SIZE] = { &PASSerror, ");
    TRAVopt(AST_IPASSES(node));
    OUT("};\n");
    return node;
}


node_st *DGPTipass(node_st *node)
{
    char *func = ID_ORIG(IPASS_NAME(node));
    if (IPASS_TARGET_FUNC(node)) {
        func = ID_ORIG(IPASS_TARGET_FUNC(node));
    }
    OUT("&%s, ", func);
    TRAVopt(IPASS_NEXT(node));
    return node;
}
