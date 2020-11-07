/**
 *
 */

#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"


static node_st *last_ste = NULL;
static node_st *first_ste = NULL;

node_st *BSTaddToST(node_st *ste, node_st *key, node_st *val)
{
    node_st *new = ASTste();
    STE_KEY(new) = key;
    STE_VALUE(new) = val;
    if (ste != NULL) {
        STE_NEXT(new) = ste;
    }
    return new;
}

void addToST(node_st *key, node_st *val)
{
    node_st *new_ste = ASTste();
    STE_KEY(new_ste) = key;
    STE_VALUE(new_ste) = val;
    if (!first_ste) {
        first_ste = new_ste;
    }
    if (last_ste) {
        STE_NEXT(last_ste) = new_ste;
    }

    last_ste = new_ste;
}

node_st *lookupST(node_st *ste, node_st *node)
{
    if (node == NULL) {
        return NULL;
    }
    node_st *curr = ste;
    while (curr) {
        if (STReq(ID_ORIG(STE_KEY(curr)), ID_ORIG(node))) {
            return STE_VALUE(curr);
        }
        curr = STE_NEXT(curr);
    }
    return NULL;
}


node_st *BSTast(node_st *node)
{
    if (AST_STABLE(node)) {
        first_ste = AST_STABLE(node);
        last_ste = AST_STABLE(node);
    }
    TRAVchildren(node);
    AST_STABLE(node) = first_ste;
    CTIabortOnError();
    return node;
}

node_st *BSTiactions(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *BSTiphase(node_st *node)
{
    if (lookupST(first_ste, IPHASE_NAME(node))) {
        CTIerror("Duplicate name: %s\n", ID_ORIG(IPHASE_NAME(node)));
    } else {
        addToST(IPHASE_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTitraversal(node_st *node)
{
    if (lookupST(first_ste, ITRAVERSAL_NAME(node))) {
        CTIerror("Duplicate name: %s\n", ID_ORIG(ITRAVERSAL_NAME(node)));
    } else {
        addToST(ITRAVERSAL_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st  *BSTitravdata(node_st *node)
{
    return node;
}


node_st *BSTipass(node_st *node)
{
    if (lookupST(first_ste, IPASS_NAME(node))) {
        CTIerror("Duplicate name: %s\n", ID_ORIG(IPASS_NAME(node)));
    } else {
        addToST(IPASS_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTinode(node_st *node)
{
    if (lookupST(first_ste, INODE_NAME(node))) {
        CTIerror("Duplicate name: %s\n", ID_ORIG(INODE_NAME(node)));
    } else {
        addToST(INODE_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTinodeset(node_st *node)
{
    if (lookupST(first_ste, INODESET_NAME(node))) {
        CTIerror("Duplicate name: %s\n", ID_ORIG(INODESET_NAME(node)));
    } else {
        addToST(INODESET_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *BSTste(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *BSTsetoperation(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *BSTsetliteral(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *BSTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *BSTattribute(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *BSTienum(node_st *node)
{
    if (lookupST(first_ste, IENUM_NAME(node))) {
        CTIerror("Duplicate name: %s\n", ID_ORIG(IENUM_NAME(node)));
    } else {
        addToST(IENUM_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTid(node_st *node)
{
    TRAVchildren(node);
    return node;
}


