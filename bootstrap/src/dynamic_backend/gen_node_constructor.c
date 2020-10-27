#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

void *DGNCallocTravData()
{
    return 0;
}

void DGNCfreeTravData(void *data)
{

}

extern bool dgif_print_semicolon;

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static long num_children = 0;
static char *node_name_upr = NULL;

node_st *DGNCast(node_st *node)
{
    dgif_print_semicolon = false;
    fp = globals.fp;
    TRAVchildren(node);
    return node;
}

node_st *DGNCinode(node_st *node)
{
    num_children = 0;
    node_st *next = INODE_NEXT(node);
    INODE_NEXT(node) = NULL;
    TRAVstart(node, TRAV_DYNAMIC_GENNODEINITFUNCTIONS);

    node_name_upr = ID_UPR(INODE_NAME(node));
    OUT_START_FUNC_FIELD();
    {
        OUT_FIELD("%s *node = NewNode()", basic_node_type);
        OUT_FIELD("node->data.N_%s = MEMmalloc(sizeof(struct NODE_DATA_%s))", ID_LWR(INODE_NAME(node)), ID_UPR(INODE_NAME(node)));
        OUT_FIELD("NODE_TYPE(node) = %s%s", "NT_", ID_UPR(INODE_NAME(node)));


        TRAVopt(INODE_ICHILDREN(node));
        TRAVopt(INODE_IATTRIBUTES(node));
        OUT_FIELD("NODE_NUMCHILDREN(node) = %ld", num_children);
        if (num_children) {
            char *name_lwr = ID_LWR(INODE_NAME(node));
            OUT_FIELD("NODE_CHILDREN(node) = node->data.N_%s->%s_children.%s_children_at", name_lwr, name_lwr, name_lwr);
        }
    }
    OUT("return node;");
    OUT_END_FUNC();
    INODE_NEXT(node) = next;
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGNCchild(node_st *node)
{
    num_children++;
    if (CHILD_IN_CONSTRUCTOR(node)) {
        OUT_FIELD("%s_%s(node) = %s", node_name_upr, ID_UPR(CHILD_NAME(node)), ID_LWR(CHILD_NAME(node)));
    } else {
        OUT_FIELD("%s_%s(node) = NULL", node_name_upr, ID_UPR(CHILD_NAME(node)));
    }
    TRAVchildren(node);
    return node;
}

node_st *DGNCattribute(node_st *node)
{
    if (ATTRIBUTE_IN_CONSTRUCTOR(node)) {
        OUT_FIELD("%s_%s(node) = %s", node_name_upr, ID_UPR(ATTRIBUTE_NAME(node)), ID_LWR(ATTRIBUTE_NAME(node)));
    } else {
        OUT_FIELD("%s_%s(node) = %s", node_name_upr, ID_UPR(ATTRIBUTE_NAME(node)), FMTattributeDefaultVal(node));
    }
    TRAVchildren(node);
    return node;
}

