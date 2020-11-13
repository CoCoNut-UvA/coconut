#include <stddef.h>
#include <stdio.h>
#include <globals.h>

#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"


static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int child_num = 0;
static char *node_type_enum_prefix = "NT_";
static node_st *ast;

node_st *DGTTast(node_st *node)
{
    fp = globals.fp;
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
    OUT("%s_vtable, ", ID_LWR(ITRAVERSAL_NAME(node)));
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}
