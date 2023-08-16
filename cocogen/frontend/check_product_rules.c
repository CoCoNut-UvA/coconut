/**
 * @file
 *
 * Traversal: checkProductRules
 * UID      : CPR
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/str.h"

static const int htable_size = 100;

void CPRinit() {
    DATA_CPR_GET()->found_children = HTnew_Ptr(htable_size);
    return;
}

static void *delete_keys(void *key, void *_element) {
    // TODO: Use C23 portable unused attribute
    (void) _element; // suppress warning
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

    return NULL;
}

static void check_attribute_reference(node_st *attribute) {
    node_st *node;
    if (ATTRIBUTE_REFERENCE_INODE(attribute) == NULL) {
        node = DATA_CPR_GET()->curr_node;
    } else {
        node = find_child(ATTRIBUTE_REFERENCE_INODE(attribute));
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
    return node;
}

/**
 * @fn CPRequation
 */
node_st *CPRequation(node_st *node) { return node; }

/**
 * @fn CPRattribute
 */
node_st *CPRattribute(node_st *node) { return node; }

/**
 * @fn CPRchild
 */
node_st *CPRchild(node_st *node) { return node; }
