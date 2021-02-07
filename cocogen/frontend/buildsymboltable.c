/**
 * @file
 *
 * Traversal that builds a symbol table mapping all identifiers to
 * their actions.
 */

#include <stddef.h>
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "globals.h"
#include "palm/hash_table.h"
#include "frontend/symboltable.h"


static node_st *last_ste = NULL;
static node_st *first_ste = NULL;
static node_st *pass_ste = NULL;


// Small helper that adds it to the global symbol table.
static void addToST(node_st *key, node_st *val)
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

node_st *BSTast(node_st *node)
{
    if (AST_STABLE(node)) {
        first_ste = AST_STABLE(node);
        last_ste = AST_STABLE(node);
    }
    TRAVchildren(node);
    AST_STABLE(node) = first_ste;
    CTIabortOnError();
    CCNfree(pass_ste);
    return node;
}

node_st *BSTiactions(node_st *node)
{
    TRAVchildren(node);
    return node;
}

// Helper to convert an ID into an info object for error reporting.
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
    node_st *ste_value = STlookup(first_ste, IPHASE_NAME(node));
    if (ste_value) {
        struct ctinfo info = {0};
        id_to_info(ID, &info);
        CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(IPHASE_NAME(node)));
    } else {
        addToST(IPHASE_NAME(node), node);
    }
    if (IPHASE_IPREFIX(node)) {
        ste_value = STlookup(first_ste, IPHASE_IPREFIX(node));
        if (ste_value) {
            struct ctinfo info;
            id_to_info(IPHASE_IPREFIX(node), &info);
            CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(IPHASE_IPREFIX(node)));
        } else {
            addToST(IPHASE_IPREFIX(node), node);
        }
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTitraversal(node_st *node)
{
    if (STlookup(first_ste, ITRAVERSAL_NAME(node))) {
        struct ctinfo info;
        id_to_info(ITRAVERSAL_NAME(node), &info);
        CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(ITRAVERSAL_NAME(node)));
    } else {
        addToST(ITRAVERSAL_NAME(node), node);
    }

    if (ITRAVERSAL_IPREFIX(node)) {
        if (STlookup(first_ste, ITRAVERSAL_IPREFIX(node))) {
            struct ctinfo info;
            id_to_info(ITRAVERSAL_IPREFIX(node), &info);
            CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(ITRAVERSAL_IPREFIX(node)));
        } else {
            addToST(ITRAVERSAL_IPREFIX(node), node);
        }
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
    if (STlookup(first_ste, IPASS_NAME(node))) {
        struct ctinfo info;
        id_to_info(IPASS_NAME(node), &info);
        CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(IPASS_NAME(node)));
    } else {
        addToST(IPASS_NAME(node), node);
    }

    if (IPASS_IPREFIX(node)) {
        if (STlookup(first_ste, IPASS_IPREFIX(node))) {
            struct ctinfo info;
            id_to_info(IPASS_IPREFIX(node), &info);
            CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(IPASS_IPREFIX(node)));
        } else {
            addToST(IPASS_IPREFIX(node), node);
        }
    }

    if (IPASS_TARGET_FUNC(node)) {
        node_st *existing_func = STlookup(pass_ste, IPASS_TARGET_FUNC(node));
        if (existing_func != NULL) {
            struct ctinfo info;
            id_to_info(IPASS_TARGET_FUNC(node), &info);
            CTIobj(CTI_ERROR, false, info, "Duplicate pass function: %s\n", IPASS_TARGET_FUNC(node));
            id_to_info(IPASS_TARGET_FUNC(existing_func), &info);
            CTIobj(CTI_NOTE, true, info, "Also defined here\n");
        } else {
            pass_ste = STadd(pass_ste, IPASS_TARGET_FUNC(node), node);
        }
    }

    TRAVchildren(node);
    return node;
}

node_st *BSTinode(node_st *node)
{
    if (STlookup(first_ste, INODE_NAME(node))) {
        struct ctinfo info;
        id_to_info(INODE_NAME(node), &info);
        CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(INODE_NAME(node)));
    } else {
        addToST(INODE_NAME(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *BSTinodeset(node_st *node)
{
    if (STlookup(first_ste, INODESET_NAME(node))) {
        struct ctinfo info;
        id_to_info(INODESET_NAME(node), &info);
        CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(INODESET_NAME(node)));
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
    if (STlookup(first_ste, IENUM_NAME(node))) {
        struct ctinfo info;
        id_to_info(IENUM_NAME(node), &info);
        CTIobj(CTI_ERROR, true, info, "Duplicate name: %s\n", ID_ORIG(IENUM_NAME(node)));
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
