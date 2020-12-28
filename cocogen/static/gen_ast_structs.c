#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"


node_st *SGNSinode(node_st *node)
{
    printf("struct %s\n", ID_ORIG(INODE_NAME(node)));
    TRAVopt(INODE_NEXT(node));
    return node;
}