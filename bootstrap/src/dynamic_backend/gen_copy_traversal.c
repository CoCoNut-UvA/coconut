
#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

void *DGCTallocTravData()
{
    return 0;
}

void DGCTfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int arg_num = 0;
static char *node_type_enum_prefix = "NT_";
static node_st *ast;

node_st *DGCTast(node_st *node)
{
    fp = stdout;
    ast = node;
    TRAVopt(AST_INODES(node));
    return node;
}

node_st *DGCTiactions(node_st *node)
{
    return node;
}

node_st *DGCTiphase(node_st *node)
{
    return node;
}

node_st *DGCTitraversal(node_st *node)
{
    return node;
}

node_st *DGCTipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGCTinode(node_st *node)
{
    arg_num = 0;
    OUT_START_FUNC("struct ccn_node *CPY%s(struct ccn_node *arg_node)", ID_LWR(INODE_NAME(node)));
    TRAVstart(node, TRAV_DYNAMIC_GENCONSTRUCTORCALL);
    OUT_FIELD("return new_node");
    OUT_END_FUNC();
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGCTinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGCTchild(node_st *node)
{

    return node;
}

node_st *DGCTattribute(node_st *node)
{

    return node;
}

node_st *DGCTste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTienum(node_st *node)
{
    return node;
}

node_st *DGCTid(node_st *node)
{
    return node;
}
