#include <stddef.h>
#include <stdio.h>
#include <globals.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

void *DGTTallocTravData()
{
    return 0;
}

void DGTTfreeTravData(void *data)
{

}

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
    OUT("ccn_free_vtable, ");
    OUT("ccn_copy_vtable, ");
    OUT("ccn_check_vtable, ");
    OUT("};\n");
    return node;
}

node_st *DGTTiactions(node_st *node)
{
    return node;
}

node_st *DGTTiphase(node_st *node)
{
    return node;
}

node_st *DGTTitraversal(node_st *node)
{
    OUT("%s_vtable, ", ID_LWR(ITRAVERSAL_NAME(node)));
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}

node_st *DGTTipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGTTinode(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGTTinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGTTchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGTTattribute(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGTTste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGTTsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGTTsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGTTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

static char *enum_prefx;
node_st *DGTTienum(node_st *node)
{
    return node;
}

node_st *DGTTid(node_st *node)
{
    return node;
}
