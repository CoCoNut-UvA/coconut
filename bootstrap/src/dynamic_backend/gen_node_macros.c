#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

void *DGNMallocTravData()
{
    return 0;
}

void DGNMfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int child_num = 0;

node_st *DGNMast(node_st *node)
{
    fp = globals.fp;
    TRAVchildren(node);
    return node;
}

node_st *DGNMinode(node_st *node)
{
    curr_node_name = ID_LWR(INODE_NAME(node));
    curr_node_name_upr = ID_UPR(INODE_NAME(node));
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    curr_node_name_upr = NULL;
    curr_node_name = NULL;
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGNMchild(node_st *node)
{
    child_num++;
    OUT("#define %s_%s(n) ((n)->data.N_%s->%s_children.%s_children_st.%s)\n", curr_node_name_upr, ID_UPR(CHILD_NAME(node)), curr_node_name, curr_node_name, curr_node_name, ID_LWR(CHILD_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGNMattribute(node_st *node)
{
    OUT("#define %s_%s(n) ((n)->data.N_%s->%s)\n", curr_node_name_upr, ID_UPR(ATTRIBUTE_NAME(node)), curr_node_name, ID_LWR(ATTRIBUTE_NAME(node)));
    TRAVchildren(node);
    return node;
}

