/**
 * @file
 *
 * Traversal: propagateAttributes
 * UID      : PRA
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "frontend/ctihelp.h"
#include "frontend/symboltable.h"
#include "palm/ctinfo.h"
#include "palm/hash_table.h"
#include "palm/str.h"
#include <assert.h>

void PRAinit() { return; }

void PRAfini() { return; }

static inline bool compare_attribute_types(node_st *a, node_st *b) {
    if (ATTRIBUTE_TYPE(a) != ATTRIBUTE_TYPE(b)) {
        return false;
    }

    if (ATTRIBUTE_TYPE(a) == AT_link || ATTRIBUTE_TYPE(a) == AT_link_or_enum ||
        ATTRIBUTE_TYPE(a) == AT_user) {
        return STReq(ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(a)),
                     ID_ORIG(ATTRIBUTE_TYPE_REFERENCE(b)));
    }

    return true;
}

/* Classic attributes can be redeclared in a node to add the constructor
   keyword. */
static bool check_classic_exception(node_st *node, node_st *parent_attr,
                                    node_st *child_attr) {
    if (ATTRIBUTE_IS_INHERITED(parent_attr) ||
        ATTRIBUTE_IS_SYNTHESIZED(parent_attr)) {
        return false;
    }

    struct ctinfo info;
    id_to_info(ATTRIBUTE_NAME(child_attr), &info);

    if (!ATTRIBUTE_IN_CONSTRUCTOR(child_attr)) {
        CTIobj(CTI_ERROR, true, info,
               "Can only redeclare attributes of parent nodesets to add the "
               "constructor keyword. Attribute '%s' of parent nodeset '%s' is "
               "redeclared in node '%s' without the constructor keyword\n",
               ID_ORIG(ATTRIBUTE_NAME(parent_attr)),
               ID_ORIG(INODESET_NAME(DATA_PRA_GET()->curr_nodeset)),
               ID_ORIG(INODESET_NAME(node)));
        CCNerrorAction();
    } else if (ATTRIBUTE_IS_INHERITED(child_attr) ||
               ATTRIBUTE_IS_SYNTHESIZED(child_attr) ||
               !compare_attribute_types(parent_attr, child_attr)) {
        CTIobj(CTI_ERROR, true, info,
               "Redeclaration of attribute '%s' in node '%s' has a different "
               "signature than the attribute in parent nodeset '%s'\n",
               ID_ORIG(ATTRIBUTE_NAME(parent_attr)),
               ID_ORIG(INODESET_NAME(node)),
               ID_ORIG(INODESET_NAME(DATA_PRA_GET()->curr_nodeset)));
        CCNerrorAction();
    }

    // TODO: Currently we don't check if lifetimes are the same.

    return true;
}

static bool check_existing_attributes(node_st *node, node_st *first_attr,
                                      node_st *new_attribute) {
    bool exists_exception = false;
    for (node_st *attribute = first_attr; attribute != NULL;
         attribute = ATTRIBUTE_NEXT(attribute)) {
        if (STReq(ID_LWR(ATTRIBUTE_NAME(attribute)),
                  ID_LWR(ATTRIBUTE_NAME(new_attribute)))) {
            if (NODE_TYPE(node) == NT_INODE &&
                check_classic_exception(node, new_attribute, attribute)) {
                exists_exception = true;
                continue;
            }

            struct ctinfo info;
            id_to_info(ATTRIBUTE_NAME(attribute), &info);
            if (NODE_TYPE(node) == NT_INODESET) {
                CTIobj(CTI_ERROR, true, info,
                       "Attribute '%s' of parent nodeset '%s' is redeclared in "
                       "nodeset '%s'\n",
                       ID_ORIG(ATTRIBUTE_NAME(attribute)),
                       ID_ORIG(INODESET_NAME(DATA_PRA_GET()->curr_nodeset)),
                       ID_ORIG(INODESET_NAME(node)));
            } else {
                CTIobj(CTI_ERROR, true, info,
                       "Inherited/synthesized attribute '%s' of parent nodeset "
                       "'%s' is redeclared in node '%s'\n",
                       ID_ORIG(ATTRIBUTE_NAME(attribute)),
                       ID_ORIG(INODESET_NAME(DATA_PRA_GET()->curr_nodeset)),
                       ID_ORIG(INODE_NAME(node)));
            }
            CCNerrorAction();
        }
    }

    return exists_exception;
}

static void check_existing_child(node_st *node, node_st *new_attribute) {
    for (node_st *child = INODE_ICHILDREN(node); child != NULL;
         child = CHILD_NEXT(child)) {
        if (STReq(ID_LWR(CHILD_NAME(child)),
                  ID_LWR(ATTRIBUTE_NAME(new_attribute)))) {
            struct ctinfo info;
            id_to_info(CHILD_NAME(child), &info);
            CTIobj(
                CTI_ERROR, true, info,
                "Name of child '%s' in node '%s' conflicts with attribute of "
                "parent nodeset '%s'\n",
                ID_ORIG(CHILD_NAME(child)), ID_ORIG(INODE_NAME(node)),
                ID_ORIG(INODESET_NAME(DATA_PRA_GET()->curr_nodeset)));
            CCNerrorAction();
        }
    }
}

static void propagate_attribute(node_st *node, node_st *attribute) {
    node_st **attribute_list;
    if (NODE_TYPE(node) == NT_INODE) {
        attribute_list = &INODE_IATTRIBUTES(node);
    } else {
        assert(NODE_TYPE(node) == NT_INODESET);
        attribute_list = &INODESET_IATTRIBUTES(node);
    }

    bool exists_exception = check_existing_attributes(node, *attribute_list, attribute);

    if (exists_exception) { // Attribute already exists, and is allowed
        return;
    }

    if (NODE_TYPE(node) == NT_INODE) {
        check_existing_child(node, attribute);
    }

    // Prepend new attribute to attribute list
    node_st *new_attribute = CCNcopy(attribute);
    ATTRIBUTE_IS_PROPAGATED(new_attribute) = true;
    ATTRIBUTE_NEXT(new_attribute) = *attribute_list;
    *attribute_list = new_attribute;
}

static void propagate_attributes(node_st *node) {
    for (node_st *attribute =
             INODESET_IATTRIBUTES(DATA_PRA_GET()->curr_nodeset);
         attribute != NULL; attribute = ATTRIBUTE_NEXT(attribute)) {
        // Skip propagated attributes
        if (ATTRIBUTE_IS_PROPAGATED(attribute)) {
            continue;
        }
        propagate_attribute(node, attribute);
    }
}

/**
 * @fn PRAast
 */
node_st *PRAast(node_st *node) {
    DATA_PRA_GET()->symboltable = AST_STABLE(node);
    TRAVinodesets(node);
    return node;
}

/**
 * @fn PRAinodeset
 */
node_st *PRAinodeset(node_st *node) {
    DATA_PRA_GET()->curr_nodeset = node;
    TRAVchildren_table(node);
    TRAVexpr(node);
    TRAVnext(node);
    return node;
}

/**
 * @fn PRAnodeset_child_entry
 */
node_st *PRAnodeset_child_entry(node_st *node) {
    propagate_attributes(NODESET_CHILD_ENTRY_REFERENCE(node));
    TRAVnext(node);
    return node;
}

/**
 * @fn PRAsetliteral
 */
node_st *PRAsetliteral(node_st *node) {
    node_st *entry =
        STlookup(DATA_PRA_GET()->symboltable, SETLITERAL_REFERENCE(node));
    assert(NODE_TYPE(entry) == NT_INODE);
    propagate_attributes(entry);
    TRAVchildren(node);
    return node;
}
