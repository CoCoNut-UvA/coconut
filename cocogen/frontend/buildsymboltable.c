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
#include "globals.h"
#include "palm/hash_table.h"


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
        if (STReq(ID_LWR(STE_KEY(curr)), ID_LWR(node))) {
            return STE_VALUE(curr);
        }
        curr = STE_NEXT(curr);
    }
    return NULL;
}


node_st *BSTast(node_st *node)
{
    exit(0);
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

void id_to_info(node_st *ID, struct ctinfo *info)
{
   struct ctinfo tmp = {.first_line = ID_ROW(ID), .filename = globals.filename,
           .line = HTlookup(globals.line_map, &ID_ROW(ID)),
           .first_column = ID_COL_BEGIN(ID), .last_column = ID_COL_END(ID)};
   *info = tmp;
}



node_st *BSTiphase(node_st *node)
{
    node_st *ID = IPHASE_NAME(node);
    node_st *ste_value = lookupST(first_ste, IPHASE_NAME(node));
    if (ste_value) {
        struct ctinfo info;
        id_to_info(ID, &info);
        CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(IPHASE_NAME(node)));
    } else {
        addToST(IPHASE_NAME(node), node);
    }
    if (IPHASE_IPREFIX(node)) {
        ste_value = lookupST(first_ste, IPHASE_IPREFIX(node));
        if (ste_value) {
            struct ctinfo info;
            id_to_info(IPHASE_IPREFIX(node), &info);
            CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(IPHASE_IPREFIX(node)));
        }
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTitraversal(node_st *node)
{
    if (lookupST(first_ste, ITRAVERSAL_NAME(node))) {
        struct ctinfo info;
        id_to_info(ITRAVERSAL_NAME(node), &info);
        CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(ITRAVERSAL_NAME(node)));
    } else {
        addToST(ITRAVERSAL_NAME(node), node);
    }

    if (ITRAVERSAL_IPREFIX(node)) {
        if (lookupST(first_ste, ITRAVERSAL_IPREFIX(node))) {
            struct ctinfo info;
            id_to_info(ITRAVERSAL_IPREFIX(node), &info);
            CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(ITRAVERSAL_IPREFIX(node)));
        }
    }
    TRAVchildren(node);
    return node;
}

node_st  *BSTitravdata(node_st *node)
{
    return node;
}

// TODO: Check pass func duplicates.
node_st *BSTipass(node_st *node)
{
    if (lookupST(first_ste, IPASS_NAME(node))) {
        struct ctinfo info;
        id_to_info(IPASS_NAME(node), &info);
        CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(IPASS_NAME(node)));
    } else {
        addToST(IPASS_NAME(node), node);
    }

    if (IPASS_IPREFIX(node)) {
        if (lookupST(first_ste, IPASS_IPREFIX(node))) {
            struct ctinfo info;
            id_to_info(IPASS_IPREFIX(node), &info);
            CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(IPASS_IPREFIX(node)));
        }
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTinode(node_st *node)
{
    if (lookupST(first_ste, INODE_NAME(node))) {
        struct ctinfo info;
        id_to_info(INODE_NAME(node), &info);
        CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(INODE_NAME(node)));
    } else {
        addToST(INODE_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTinodeset(node_st *node)
{
    if (lookupST(first_ste, INODESET_NAME(node))) {
        struct ctinfo info;
        id_to_info(INODESET_NAME(node), &info);
        CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(INODESET_NAME(node)));
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
        struct ctinfo info;
        id_to_info(IENUM_NAME(node), &info);
        CTIerrorObj(&info, "Duplicate name: %s\n", ID_ORIG(IENUM_NAME(node)));
    } else {
        addToST(IENUM_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTid(node_st *node)
{
    return node;
}

node_st *BSTilifetime(node_st *node)
{
    return node;
}

node_st *BSTlifetime_range(node_st *node)
{
    return node;
}
