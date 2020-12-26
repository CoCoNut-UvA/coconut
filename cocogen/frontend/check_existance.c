#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"

extern node_st *lookupST(node_st*, node_st*);

node_st *BSTaddToST(node_st *ste, node_st *key, node_st *val);
static bool seen_root_node = false;
static bool seen_start_phase = false;
static node_st *ste = NULL;
static node_st *node_ste = NULL;
static node_st *root = NULL;
static node_st *enum_prefix_ste = NULL;

// TODO: Check UID.
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

node_st  *CEXitravdata(node_st *node)
{
    // TODO: Check if double decl in travdata.
    if (ITRAVDATA_TYPE(node) == AT_link_or_enum) {
        node_st *ste_entry = lookupST(ste, ITRAVDATA_TYPE_REFERENCE(node));
        if (ste_entry == NULL) {
            CTIerror("Could not find type reference for travdata entry.");
        } else {
            if (NODE_TYPE(ste_entry) == NT_INODE || NODE_TYPE(ste_entry) == NT_INODESET) {
                ITRAVDATA_TYPE(node) = AT_link;
            } else if (NODE_TYPE(ste_entry) == NT_IENUM) {
                ITRAVDATA_TYPE(node) = AT_link_or_enum;
            } else {
                CTIerror("Invalid type in travdata entry.");
            }
        }
    }

    TRAVopt(ITRAVDATA_NEXT(node));
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
    node_st *ref = lookupST(ste, CHILD_TYPE_REFERENCE(node));
    if (!ref) {
        CTIerror("Child type %s is not defined.", ID_ORIG(CHILD_TYPE_REFERENCE(node)));
    }
    if (NODE_TYPE(ref) == NT_INODESET) {
        CHILD_TYPE(node) = CT_inodeset;
    } else if(NODE_TYPE(ref) == NT_INODE) {
        CHILD_TYPE(node) = CT_inode;
    } else {
        CTIerror("Child is not a node or nodeset: %s\n", ID_ORIG(CHILD_NAME(node)));
    }
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
    node_st *ref_node = lookupST(ste, SETREFERENCE_REFERENCE(node));
    if (!ref_node) {
        CTIerror("Set reference is undefined: %s\n", ID_ORIG(SETREFERENCE_REFERENCE(node)));
    } else if (NODE_TYPE(ref_node) != NT_INODESET) {
        CTIerror("Set reference is not a reference to a nodeset: %s\n", ID_ORIG(SETREFERENCE_REFERENCE(node)));
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
            CTIerror("Could not find type reference for attribute.");
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

// TODO: Check enum prefix to reserver prefixes.
node_st *CEXienum(node_st *node)
{
    if (lookupST(enum_prefix_ste, IENUM_IPREFIX(node))) {
        CTIerror("Double declaration of enum prefix: %s\n", ID_ORIG(IENUM_IPREFIX(node)));
    } else {
        enum_prefix_ste = BSTaddToST(enum_prefix_ste, IENUM_IPREFIX(node), node);
    }
    TRAVchildren(node);
    return node;
}

node_st *CEXid(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *CEXilifetime(node_st *node)
{
    return node;
}

node_st *CEXlifetime_range(node_st *node)
{
    return node;
}
