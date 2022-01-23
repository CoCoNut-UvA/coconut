#include <stddef.h>
#include <stdio.h>
#include <globals.h>

#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"



static node_st *ast;

node_st *DGTTast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    ast = node;
    OUT("const ccn_trav_ft *ccn_trav_vtable[_TRAV_SIZE] = { ccn_error_vtable, ");
    TRAVopt(AST_ITRAVERSALS(node));
    // Order is important and should match the one in the TRAV enums.
    OUT("ccn_free_vtable, ");
    OUT("ccn_check_vtable, ");
    OUT("ccn_copy_vtable, ");
    OUT("};\n");
    return node;
}


node_st *DGTTitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("%s_vtable, ", ID_LWR(ITRAVERSAL_NAME(node)));
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}
