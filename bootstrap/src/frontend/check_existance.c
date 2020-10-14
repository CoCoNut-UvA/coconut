#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"

extern node_st *lookupST(node_st*, node_st*);

void *CEXallocTravData()
{
    return 0;
}

void CEXfreeTravData(void *data)
{

}

node_st *BSTaddToST(node_st *ste, node_st *key, node_st *val);
static bool seen_root_node = false;
static bool seen_start_phase = false;
static node_st *ste = NULL;
static node_st *node_ste = NULL;
static node_st *root = NULL;


node_st *CEXast(node_st *node)
{
    root = node;
    ste = AST_STABLE(node);
    TRAVchildren(node);
    CTIabortOnError();
    return node;
}

node_st *CEXiactions(node_st *node)
{
    TRAVchildren(node);
    node_st *ref = lookupST(ste, IACTIONS_REFERENCE(node));
    if (ref == NULL) {
        CTIerror("Could not find action: %s\n", ID_ORIG(IACTIONS_REFERENCE(node)));
    }
    return node;
}

node_st *CEXiphase(node_st *node)
{
    if (IPHASE_IS_START(node)) {
        if (seen_start_phase) {
            CTIerror("Double definition of a starting phase.");
        } else {
            AST_START_PHASE(root) = node;
            seen_start_phase = true;
        }
    }
    TRAVchildren(node);
    return node;
}

node_st *CEXitraversal(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *CEXipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *CEXinode(node_st *node)
{
    if (INODE_IS_ROOT(node)) {
        if (!seen_root_node) {
            AST_ROOT_NODE(root) = node;
            seen_root_node = true;
        } else {
            CTIerror("Double definition of root node");
        }
    }
    TRAVopt(INODE_IATTRIBUTES(node));
    TRAVopt(INODE_ICHILDREN(node));
    if (node_ste != NULL) {
        TRAVstart(node_ste, TRAV_free);
        node_ste = NULL;
    }
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *CEXinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *CEXchild(node_st *node)
{
    if (lookupST(node_ste, CHILD_NAME(node)) == NULL) {
        node_ste = BSTaddToST(node_ste, CHILD_NAME(node), node);
    } else {
        CTIerror("Double declaration of child name: %s\n", ID_ORIG(CHILD_NAME(node)));
    }
    TRAVchildren(node);
    return node;
}

node_st *CEXste(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *CEXsetoperation(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *CEXsetreference(node_st *node)
{
    if (lookupST(ste, SETREFERENCE_REFERENCE(node)) == NULL) {
        CTIerror("Set reference is undefined: %s\n", ID_ORIG(SETREFERENCE_REFERENCE(node)));
    }
    TRAVchildren(node);
    return node;
}

node_st *CEXsetliteral(node_st *node)
{
    if (lookupST(ste, SETLITERAL_REFERENCE(node)) == NULL) {
        CTIerror("Set member is undefined: %s\n", ID_ORIG(SETLITERAL_REFERENCE(node)));
    }
    TRAVchildren(node);
    return node;
}

node_st *CEXattribute(node_st *node)
{
    if (lookupST(node_ste, ATTRIBUTE_NAME(node)) == NULL) {
        node_ste = BSTaddToST(node_ste, ATTRIBUTE_NAME(node), node);
    } else {
        CTIerror("Double declaration of attribute name: %s\n", ID_ORIG(ATTRIBUTE_NAME(node)));
    }

    if (ATTRIBUTE_TYPE(node) == AT_link_or_enum) {
        node_st *ste_entry = lookupST(ste, ATTRIBUTE_TYPE_REFERENCE(node));
        if (ste_entry == NULL) {
            CTIerror("COuld not find type reference for attribute.");
        } else {
            if (NODE_TYPE(ste_entry) == NT_INODE || NODE_TYPE(ste_entry) == NT_INODESET) {
                ATTRIBUTE_TYPE(node) = AT_link;
            } else if (NODE_TYPE(ste_entry) == NT_IENUM) {
                ATTRIBUTE_TYPE(node) = AT_link_or_enum;
            } else {
                CTIerror("Invalid type in attribute type.");
            }
        }
    }

    TRAVchildren(node);
    return node;
}

node_st *CEXienum(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *CEXid(node_st *node)
{
    TRAVchildren(node);
    return node;
}
