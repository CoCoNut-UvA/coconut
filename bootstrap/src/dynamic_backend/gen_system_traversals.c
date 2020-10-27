
#include <stddef.h>
#include <stdio.h>
#include <globals.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

void *DGSTallocTravData()
{
    return 0;
}

void DGSTfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static char *prefix = NULL;
static node_st *ast;

node_st *DGSTast(node_st *node)
{
    fp = globals.fp;
    ast = node;
    OUT("const ccn_trav_ft ccn_copy_vtable[_NT_SIZE] = { &TRAVerror, ");
    prefix = "CPY";
    TRAVopt(AST_INODES(node));
    OUT("};\n");

    OUT("const ccn_trav_ft ccn_check_vtable[_NT_SIZE] = { &TRAVerror, ");
    prefix = "CHK";
    TRAVopt(AST_INODES(node));
    OUT("};\n");

    OUT("const ccn_trav_ft ccn_free_vtable[_NT_SIZE] = { &TRAVerror, ");
    prefix = "DEL";
    TRAVopt(AST_INODES(node));
    OUT("};\n");

    OUT("const ccn_trav_ft ccn_error_vtable[_NT_SIZE] = {&TRAVerror, ");
    prefix = "ERR";
    TRAVopt(AST_INODES(node));
    OUT("};\n");

    return node;
}

node_st *DGSTinode(node_st *node)
{
    // Important, OUT should be before the traversal.
    if (STReq(prefix, "ERR")) {
        OUT("&TRAVerror,");
    } else {
        OUT("&%s%s, ", prefix, ID_LWR(INODE_NAME(node)));
    }
    TRAVopt(INODE_NEXT(node));

    return node;
}

