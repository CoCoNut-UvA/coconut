/**
 * @file
 *
 * Traversal: checkProductRules
 * UID      : CPR
 *
 * TODO: prevent duplicate production rules / args
 */

#include <assert.h>

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "frontend/ctihelp.h"
#include "frontend/symboltable.h"
#include "palm/ctinfo.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/str.h"

static const int htable_size = 100;

void CPRinit() {
    DATA_CPR_GET()->found_children = HTnew_Ptr(htable_size);
    DATA_CPR_GET()->found_attributes = HTnew_Ptr(htable_size);
    return;
}

static void *delete_keys(void *key, void *_element) {
    // TODO: Use C23 portable unused attribute
    (void)_element; // suppress warning
    return MEMfree(key);
}

static void *delete_subtables(void *htable) {
    HTmapWithKey(htable, delete_keys);
    HTdelete((htable_st *)htable);
    return NULL;
}

void CPRfini() {
    HTmap(DATA_CPR_GET()->found_children, delete_subtables);
    HTdelete(DATA_CPR_GET()->found_children);
    HTmap(DATA_CPR_GET()->found_attributes, delete_subtables);
    HTdelete(DATA_CPR_GET()->found_attributes);
    return;
}

static node_st *HTfind_nested(htable_st *table, node_st *node, char *key) {
    htable_st *subtable = HTlookup(table, node);
    if (subtable == NULL) {
        return false;
    }

    return (node_st *)HTlookup(subtable, key);
}

static bool HTlookup_nested(htable_st *table, node_st *node, char *key) {
    return HTfind_nested(table, node, key) != NULL;
}

static void HTinsert_nested(htable_st *table, node_st *node, char *key,
                            node_st *value) {
    htable_st *subtable = HTlookup(table, node);
    if (subtable == NULL) {
        subtable = HTnew_String(htable_size);
        HTinsert(table, node, subtable);
    }

    if (HTlookup(subtable, key) == NULL) {
        HTinsert(subtable, STRcpy(key), (void *)value);
    }
}

static node_st *find_child(node_st *child_id) {
    node_st *cached =
        HTfind_nested(DATA_CPR_GET()->found_children, DATA_CPR_GET()->curr_node,
                      ID_LWR(child_id));
    if (cached != NULL) {
        return cached;
    }

    for (node_st *can = INODE_ICHILDREN(DATA_CPR_GET()->curr_node); can != NULL;
         can = CHILD_NEXT(can)) {
        if (STReq(ID_LWR(CHILD_NAME(can)), ID_LWR(child_id))) {
            HTinsert_nested(DATA_CPR_GET()->found_children,
                            DATA_CPR_GET()->curr_node, ID_LWR(child_id), can);
            return can;
        }
    }

    return NULL;
}

static node_st *find_attribute(node_st *node, node_st *attr_list,
                               node_st *attribute_id) {
    node_st *cached = HTfind_nested(DATA_CPR_GET()->found_attributes, node,
                                    ID_LWR(attribute_id));
    if (cached != NULL) {
        return cached;
    }

    for (node_st *can = attr_list; can != NULL; can = ATTRIBUTE_NEXT(can)) {
        if (STReq(ID_LWR(ATTRIBUTE_NAME(can)), ID_LWR(attribute_id))) {
            HTinsert_nested(DATA_CPR_GET()->found_attributes, node,
                            ID_LWR(attribute_id), can);
            return can;
        }
    }

    return NULL;
}

static node_st *find_product_rule(node_st *equation_list, node_st *node_name,
                                  node_st *attribute_name) {
    /* TODO: might want to cache product rules in hash table so we don't loop
       over entries all the time. Then we can check
       htable[eq_list][node_name][attr_name] on subsequent calls. */
    for (node_st *eq = equation_list; eq != NULL; eq = EQUATION_NEXT(eq)) {
        node_st *can = EQUATION_RULE(eq);
        node_st *can_node = ATTRIBUTE_REFERENCE_INODE(can);
        // Check same node (NULL == this)
        if ((node_name == NULL && can_node == NULL) ||
            (node_name != NULL && can_node != NULL &&
             STReq(ID_LWR(node_name), ID_LWR(can_node)))) {
            // Check same attribute
            if (STReq(ID_LWR(ATTRIBUTE_REFERENCE_IATTRIBUTE(can)),
                      ID_LWR(attribute_name))) {
                return can;
            }
        }
    }

    return NULL;
}

static void check_attribute_reference(node_st *node, bool in_rule) {
    node_st *ref_node, *attribute, *node_name;
    if (ATTRIBUTE_REFERENCE_INODE(node) == NULL) {
        ref_node = DATA_CPR_GET()->curr_node;
        ATTRIBUTE_REFERENCE_NODE_TYPE(node) = INODE_NAME(ref_node);
    } else {
        node_st *child = find_child(ATTRIBUTE_REFERENCE_INODE(node));
        if (child == NULL) {
            struct ctinfo info;
            id_to_info(ATTRIBUTE_REFERENCE_INODE(node), &info);
            CTIobj(CTI_ERROR, true, info, "Referenced child does not exist\n");
            CCNerrorAction();
            return;
        }
        ATTRIBUTE_REFERENCE_CHILD_REFERENCE(node) = child;
        ATTRIBUTE_REFERENCE_NODE_TYPE(node) = CHILD_TYPE_REFERENCE(child);
        ref_node =
            STlookup(DATA_CPR_GET()->symboltable, CHILD_TYPE_REFERENCE(child));
        assert(ref_node != NULL);
    }

    if (NODE_TYPE(ref_node) == NT_INODE) {
        attribute = find_attribute(ref_node, INODE_IATTRIBUTES(ref_node),
                                   ATTRIBUTE_REFERENCE_IATTRIBUTE(node));
        node_name = INODE_NAME(ref_node);
    } else {
        assert(NODE_TYPE(ref_node) == NT_INODESET);

        attribute = find_attribute(ref_node, INODESET_IATTRIBUTES(ref_node),
                                   ATTRIBUTE_REFERENCE_IATTRIBUTE(node));
        node_name = INODESET_NAME(ref_node);
    }

    if (attribute == NULL) {
        struct ctinfo info;
        id_to_info(ATTRIBUTE_REFERENCE_IATTRIBUTE(node), &info);
        CTIobj(CTI_ERROR, true, info,
               "Node(set) '%s' does not contain referenced attribute\n",
               ID_ORIG(node_name));
        CCNerrorAction();
        return;
    }

    ATTRIBUTE_REFERENCE_REFERENCE(node) = attribute;

    if (in_rule) {
        if (ATTRIBUTE_REFERENCE_INODE(node) == NULL &&
            !ATTRIBUTE_IS_SYNTHESIZED(attribute)) {
            struct ctinfo info;
            id_to_info(ATTRIBUTE_REFERENCE_IATTRIBUTE(node), &info);
            CTIobj(CTI_ERROR, true, info,
                   "Product rule does not exist: 'this.%s' is not a "
                   "synthesized attribute\n",
                   ID_ORIG(ATTRIBUTE_NAME(attribute)));
            CCNerrorAction();
        } else if (ATTRIBUTE_REFERENCE_INODE(node) != NULL &&
                   !ATTRIBUTE_IS_INHERITED(attribute)) {
            struct ctinfo info;
            id_to_info(ATTRIBUTE_REFERENCE_IATTRIBUTE(node), &info);
            CTIobj(CTI_ERROR, true, info,
                   "Product rule does not exist: '%s.%s' is not an inherited "
                   "attribute\n",
                   ID_ORIG(ATTRIBUTE_REFERENCE_INODE(node)),
                   ID_ORIG(ATTRIBUTE_NAME(attribute)));
            CCNerrorAction();
        }
    }
}

static void check_product_rule(node_st *equations, node_st *node_name,
                               node_st *attribute_name) {
    if (find_product_rule(equations, node_name, attribute_name) == NULL) {
        struct ctinfo info;
        id_to_info(INODE_NAME(DATA_CPR_GET()->curr_node), &info);
        char *node_id = node_name == NULL ? "this" : ID_ORIG(node_name);
        CTIobj(CTI_ERROR, true, info,
               "Node '%1$s' is missing attribute equation dependencies for the "
               "product rule of '%2$s.%3$s'. If no dependencies are necessary, "
               "add %2$s.%3$s = {args = {}} to the equations block \n",
               ID_ORIG(INODE_NAME(DATA_CPR_GET()->curr_node)), node_id,
               ID_ORIG(attribute_name));
        CCNerrorAction();
    }
}

/**
 * @fn CPRast
 */
node_st *CPRast(node_st *node) {
    DATA_CPR_GET()->symboltable = AST_STABLE(node);
    TRAVinodes(node);
    return node;
}

/**
 * @fn CPRinode
 */
node_st *CPRinode(node_st *node) {
    DATA_CPR_GET()->curr_node = node;
    TRAViequations(node);
    TRAVichildren(node);
    TRAViattributes(node);
    TRAVnext(node);
    return node;
}

/**
 * @fn CPRequation
 */
node_st *CPRequation(node_st *node) {
    check_attribute_reference(EQUATION_RULE(node), true);
    TRAViargs(node);
    TRAVnext(node);
    return node;
}

/**
 * @fn CPRequation
 */
node_st *CPRequation_dependency(node_st *node) {
    check_attribute_reference(EQUATION_DEPENDENCY_IATTRIBUTE(node), false);
    TRAVnext(node);
    return node;
}

/**
 * @fn CPRattribute
 */
node_st *CPRattribute(node_st *node) {
    if (ATTRIBUTE_IS_SYNTHESIZED(node)) {
        check_product_rule(INODE_IEQUATIONS(DATA_CPR_GET()->curr_node), NULL,
                           ATTRIBUTE_NAME(node));
    }

    TRAVnext(node);
    return node;
}

/**
 * @fn CPRchild
 */
node_st *CPRchild(node_st *node) {
    node_st *ref_node =
        STlookup(DATA_CPR_GET()->symboltable, CHILD_TYPE_REFERENCE(node));
    assert(ref_node != NULL);
    node_st *attributes;
    if (NODE_TYPE(ref_node) == NT_INODE) {
        attributes = INODE_IATTRIBUTES(ref_node);
    } else {
        assert(NODE_TYPE(ref_node) == NT_INODESET);
        attributes = INODESET_IATTRIBUTES(ref_node);
    }

    for (node_st *attribute = attributes; attribute != NULL;
         attribute = ATTRIBUTE_NEXT(attribute)) {
        if (ATTRIBUTE_IS_INHERITED(attribute)) {
            check_product_rule(INODE_IEQUATIONS(DATA_CPR_GET()->curr_node),
                               CHILD_NAME(node), ATTRIBUTE_NAME(attribute));
        }
    }

    TRAVnext(node);
    return node;
}
