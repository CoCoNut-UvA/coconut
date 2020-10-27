#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "globals.h"

void *DGBUallocTravData()
{
    return 0;
}

void DGBUfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int arg_num = 0;

node_st *dynamic_genBaseNodeInit(node_st *root)
{
    indent = 0;
    fp = globals.fp;
    OUT_START_FUNC("%s *NewNode()", basic_node_type);
    OUT_FIELD("%s *node = MEMmalloc(sizeof(%s))", basic_node_type, basic_node_type);
    OUT_FIELD("NODE_TYPE(node) = NT_NULL");
    OUT_FIELD("NODE_CHILDREN(node) = NULL");
    OUT_FIELD("NODE_NUMCHILDREN(node) = 0");
    OUT_FIELD("return node");
    OUT_END_FUNC();

    return root;
}


node_st *dynamic_genBaseNode(node_st *root)
{
    indent = 0;
    fp = globals.fp;
    OUT("#define NODE_TYPE(n) ((n)->nodetype)\n");
    OUT("#define NODE_CHILDREN(n) ((n)->children)\n");
    OUT("#define NODE_NUMCHILDREN(n) ((n)->num_children)\n");
    OUT_STRUCT("ccn_node");
    OUT_FIELD("enum ccn_nodetype nodetype");
    OUT_FIELD("union NODE_DATA data");
    OUT_FIELD("struct ccn_node **children");
    OUT_FIELD("long int num_children");
    OUT_STRUCT_END();

    return root;
}

node_st *DGBUast(node_st *node)
{
    fp = globals.fp;
    indent = 0;
    OUT_UNION("NODE_DATA");
    TRAVchildren(node);
    OUT_STRUCT_END();
    return node;
}


node_st *DGBUinode(node_st *node)
{
    TRAVopt(INODE_NEXT(node));
    OUT_FIELD("struct NODE_DATA_%s *N_%s", ID_UPR((INODE_NAME(node))), ID_LWR(INODE_NAME(node)));
    return node;
}
