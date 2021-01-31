#include <stddef.h>
#include "globals.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"

extern node_st *lookupST(node_st*, node_st*);
extern void id_to_info(node_st *ID, struct ctinfo *info);
extern node_st *BSTaddToST(node_st *ste, node_st *key, node_st *val);
static bool seen_root_node = false;
static bool seen_start_phase = false;
static node_st *ste = NULL;
static node_st *node_ste = NULL;
static node_st *root = NULL;
static node_st *enum_prefix_ste = NULL;
static node_st *trav_data_names = NULL;


char *uid_reserved[] = {
    "TRAV",
    "TRV",
    "CCN",
    "PASS",
    "CPY",
    "CHK",
    "DEL",
};

void checkUID(node_st *id)
{
    if (id == NULL) {
        return;
    }
    for (unsigned int i = 0; i < sizeof(uid_reserved)/sizeof(uid_reserved[0]); i++) {
        if (STReq(ID_UPR(id), uid_reserved[i])) {
            CTI(CTI_ERROR, true, "Uid is reversed: %s\n", uid_reserved[i]);
        }
    }
}

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
        CTI(CTI_ERROR, true, "Could not find action: %s\n", ID_ORIG(IACTIONS_REFERENCE(node)));
    }
    return node;
}

node_st *CEXiphase(node_st *node)
{
    checkUID(IPHASE_IPREFIX(node));
    if (IPHASE_IS_START(node)) {
        if (seen_start_phase) {
            CTI(CTI_ERROR, true, "Double definition of a starting phase.");
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
    checkUID(ITRAVERSAL_IPREFIX(node));
    if (trav_data_names != NULL) {
        CCNfree(trav_data_names);
    }
    trav_data_names = NULL;
    TRAVchildren(node);
    return node;
}

node_st  *CEXitravdata(node_st *node)
{
    node_st *poss_conflict = lookupST(trav_data_names, ITRAVDATA_NAME(node));
    if (poss_conflict != NULL) {
        struct ctinfo info = {.filename = globals.filename, .first_line = ID_ROW(ITRAVDATA_NAME(node))};
        CTIobj(CTI_ERROR, false, info, "Name(%s) is used multiple times in travdata.", ID_ORIG(ITRAVDATA_NAME(node)));
        struct ctinfo conf_info = {.filename = globals.filename, .first_line = ID_ROW((poss_conflict))};
        CTIobj(CTI_NOTE, true, conf_info, "Used here as well.");
    } else {
        trav_data_names = BSTaddToST(trav_data_names, ITRAVDATA_NAME(node), ITRAVDATA_NAME(node));
    }

    if (ITRAVDATA_TYPE(node) == AT_link_or_enum) {
        node_st *ste_entry = lookupST(ste, ITRAVDATA_TYPE_REFERENCE(node));
        if (ste_entry == NULL) {
            CTI(CTI_ERROR, true, "Could not find type reference for travdata entry.");
        } else {
            if (NODE_TYPE(ste_entry) == NT_INODE || NODE_TYPE(ste_entry) == NT_INODESET) {
                ITRAVDATA_TYPE(node) = AT_link;
            } else if (NODE_TYPE(ste_entry) == NT_IENUM) {
                ITRAVDATA_TYPE(node) = AT_link_or_enum;
            } else {
                CTI(CTI_ERROR, true, "Invalid type in travdata entry.");
            }
        }
    }

    TRAVopt(ITRAVDATA_NEXT(node));
    return node;
}

node_st *CEXipass(node_st *node)
{
    checkUID(IPASS_IPREFIX(node));
    TRAVchildren(node);
    return node;
}

node_st *CEXinode(node_st *node)
{
    if (STReq(ID_LWR(INODE_NAME(node)), "node")) {
        CTI(CTI_ERROR, true, "Can not call node 'node'.");
    }
    if (INODE_IS_ROOT(node)) {
        if (!seen_root_node) {
            AST_ROOT_NODE(root) = node;
            seen_root_node = true;
        } else {
            CTI(CTI_ERROR, true, "Double definition of root node");
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
        struct ctinfo info;
        id_to_info(CHILD_TYPE_REFERENCE(node), &info);
        CTIobj(CTI_ERROR, true, info, "Child type %s is not defined.", ID_ORIG(CHILD_TYPE_REFERENCE(node)));
    }
    if (NODE_TYPE(ref) == NT_INODESET) {
        CHILD_TYPE(node) = CT_inodeset;
    } else if(NODE_TYPE(ref) == NT_INODE) {
        CHILD_TYPE(node) = CT_inode;
    } else {
        struct ctinfo info;
        id_to_info(CHILD_NAME(node), &info);
        CTIobj(CTI_ERROR, true, info, "Child is not a node or nodeset: %s\n", ID_ORIG(CHILD_NAME(node)));
    }
    if (lookupST(node_ste, CHILD_NAME(node)) == NULL) {
        node_ste = BSTaddToST(node_ste, CHILD_NAME(node), node);
    } else {
        struct ctinfo info;
        id_to_info(CHILD_NAME(node), &info);
        CTIobj(CTI_ERROR, true, info, "Double declaration of attribute/child name: %s\n", ID_ORIG(CHILD_NAME(node)));
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
        CTI(CTI_ERROR, true, "Set reference is undefined: %s\n", ID_ORIG(SETREFERENCE_REFERENCE(node)));
    } else if (NODE_TYPE(ref_node) != NT_INODESET) {
        CTI(CTI_ERROR, true, "Set reference is not a reference to a nodeset: %s\n", ID_ORIG(SETREFERENCE_REFERENCE(node)));
    }
    TRAVchildren(node);
    return node;
}

node_st *CEXsetliteral(node_st *node)
{
    if (lookupST(ste, SETLITERAL_REFERENCE(node)) == NULL) {
        CTI(CTI_ERROR, true, "Set member is undefined: %s\n", ID_ORIG(SETLITERAL_REFERENCE(node)));
    }
    TRAVchildren(node);
    return node;
}

node_st *CEXattribute(node_st *node)
{
    if (lookupST(node_ste, ATTRIBUTE_NAME(node)) == NULL) {
        node_ste = BSTaddToST(node_ste, ATTRIBUTE_NAME(node), node);
    } else {
        CTI(CTI_ERROR, true, "Double declaration of attribute/child name: %s\n", ID_ORIG(ATTRIBUTE_NAME(node)));
    }

    if (ATTRIBUTE_TYPE(node) == AT_link_or_enum) {
        node_st *ste_entry = lookupST(ste, ATTRIBUTE_TYPE_REFERENCE(node));
        if (ste_entry == NULL) {
            struct ctinfo info;
            id_to_info(ATTRIBUTE_TYPE_REFERENCE(node), &info);
            CTIobj(CTI_ERROR, true, info, "Could not find type reference for attribute with type %s.", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(node)));
        } else {
            if (NODE_TYPE(ste_entry) == NT_INODE || NODE_TYPE(ste_entry) == NT_INODESET) {
                ATTRIBUTE_TYPE(node) = AT_link;
            } else if (NODE_TYPE(ste_entry) == NT_IENUM) {
                ATTRIBUTE_TYPE(node) = AT_link_or_enum;
            } else {
                CTI(CTI_ERROR, true, "Invalid type in attribute type: %s", ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(node)));
            }
        }
    }

    TRAVchildren(node);
    return node;
}

char *preserved_enum_prefix[] = {
    "NT",
    "NS",
    "TRAV",
    "PASS",
    "CCN"
};

node_st *CEXienum(node_st *node)
{
    for (unsigned int i = 0; i < sizeof(preserved_enum_prefix)/sizeof(preserved_enum_prefix[0]); i++) {
        if (STReq(preserved_enum_prefix[i], ID_UPR(IENUM_IPREFIX(node)))) {
            CTI(CTI_ERROR, true, "Enum prefix %s is reversed.", preserved_enum_prefix[i]);
        }
    }
    if (lookupST(enum_prefix_ste, IENUM_IPREFIX(node))) {
        CTI(CTI_ERROR, true, "Double declaration of enum prefix: %s\n", ID_ORIG(IENUM_IPREFIX(node)));
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
