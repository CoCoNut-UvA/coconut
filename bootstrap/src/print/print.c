#include <stdio.h>

#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"

void *PRTallocTravData()
{
    return 0;
}

void PRTfreeTravData(void *data)
{

}

node_st *PRTast(node_st *ast)
{
    printf("AST:\n");
    TRAVchildren(ast);
    return ast;
}

node_st *PRTiphase(node_st *node)
{
    printf("Phase: %s\n", ID_ORIG(IPHASE_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *PRTitraversal(node_st *node)
{
    printf("Trav: %s\n", ID_ORIG(ITRAVERSAL_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *PRTipass(node_st *node)
{
    printf("Pass: %s\n", ID_ORIG(IPASS_NAME(node)));
    TRAVchildren(node);
    return node;
}


node_st *PRTiactions(node_st *node)
{
    TRAVchildren(node);
    return node;
}


node_st *PRTinode(node_st *node)
{
    printf("NODE: %s\n", ID_ORIG(INODE_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *PRTchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *PRTattribute(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *PRTinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *PRTid(node_st *node)
{
    printf("Ref: %s\n", ID_ORIG(node));
    return node;
}

node_st *PRTsetoperation(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *PRTienum(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *PRTsetliteral(node_st *node)
{
    TRAVchildren(node);
    return node;
}
