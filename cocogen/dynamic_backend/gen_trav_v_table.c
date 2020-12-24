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
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int child_num = 0;
static char *node_type_enum_prefix = "NT_";
static node_st *ast;

node_st *DGTVast(node_st *node)
{
    fp = globals.fp;
    ast = node;
    OUT("#include \"ccn/dynamic_core.h\"\n");
    TRAVopt(AST_ITRAVERSALS(node));
    return node;
}

extern node_st *curr_trav_nodes;
extern node_st *curr_trav_prefix;
extern int curr_trav_index;
node_st *DGTVitraversal(node_st *node)
{
    curr_trav_prefix = ITRAVERSAL_IPREFIX(node);
    curr_trav_nodes = ITRAVERSAL_INODES(node);
    OUT("const ccn_trav_ft %s_vtable[_NT_SIZE] = { &TRAVerror, ", ID_LWR(ITRAVERSAL_NAME(node)));
    curr_trav_index = ITRAVERSAL_INDEX(node);
    TRAVstart(AST_INODES(ast), TRAV_DYNAMIC_GENNODESFORTRAVTABLE);
    OUT("};\n");
    TRAVchildren(node);
    return node;
}

