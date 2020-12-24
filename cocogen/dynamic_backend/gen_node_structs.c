#include <stdio.h>

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"


static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static int child_num = 0;

node_st *DGNSast(node_st *node)
{
    fp = globals.fp;
    TRAVchildren(node);
    return node;
}

node_st *DGNSinode(node_st *node)
{
    char *name_upr = ID_UPR(INODE_NAME(node));
    char *name_lwr = ID_LWR(INODE_NAME(node));
    OUT_STRUCT("NODE_DATA_%s", name_upr);
    {
        if (INODE_ICHILDREN(node)) {
            OUT_UNION("NODE_CHILDREN_%s", name_upr);
            OUT_STRUCT("NODE_CHILDREN_%s_STRUCT", name_upr);
            child_num = 0;
            TRAVdo(INODE_ICHILDREN(node));
            OUT_TYPEDEF_STRUCT_END("%s_children_st", name_lwr);
            OUT_FIELD("%s *%s_children_at[%ld]", basic_node_type, name_lwr, child_num);
            OUT_TYPEDEF_STRUCT_END("%s_children", name_lwr);
        }

        TRAVopt(INODE_IATTRIBUTES(node));
    }
    OUT_STRUCT_END();

    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGNSchild(node_st *node)
{
    child_num++;
    OUT_FIELD("%s *%s", basic_node_type, ID_LWR(CHILD_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGNSattribute(node_st *node)
{
    if (ATTRIBUTE_TYPE(node) == AT_link) {
        OUT_FIELD("%s *%s", basic_node_type, ID_LWR(ATTRIBUTE_NAME(node)));
    } else if (ATTRIBUTE_TYPE(node) == AT_link_or_enum) {
        OUT_FIELD("enum %s %s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(node)), ID_LWR(ATTRIBUTE_NAME(node)));
    } else {
        OUT_FIELD("%s %s", FMTattributeTypeToString(ATTRIBUTE_TYPE(node)), ID_LWR(ATTRIBUTE_NAME(node)));
    }
    TRAVchildren(node);
    return node;
}
