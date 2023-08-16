/**
 * @file
 *
 * Traversal: nodesetChildTable
 * UID      : NCT
 *
 *
 */

#include <assert.h>
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "palm/hash_table.h"
#include "palm/ctinfo.h"
#include "frontend/symboltable.h"
#include "frontend/ctihelp.h"

static const int htable_size = 100;

enum ht_state {
    ht_null = 0,
    ht_working = 1,
    ht_finished = 2
};

static enum ht_state HTNCT_lookup(htable_st *table, void *key) {
    return (enum ht_state) HTlookup(table, key);
}

static bool HTNCT_insert(htable_st *table, void *key, enum ht_state value) {
    return HTinsert(table, key, (void *) value);
}

void NCTinit() {
    DATA_NCT_GET()->built_tables = HTnew_Ptr(htable_size);
    return;
}
void NCTfini() {
    HTdelete(DATA_NCT_GET()->built_tables);
    return;
}

static void add_inheritance_entry(node_st *reference) {
    // Check if already added
    if (HTlookup(DATA_NCT_GET()->added_refs, reference)) {
        return;
    }
    node_st *new_node = ASTnodeset_child_entry(reference);

    if (DATA_NCT_GET()->last_entry == NULL) {
        INODESET_CHILDREN_TABLE(DATA_NCT_GET()->curr_set) = new_node;
    } else {
        NODESET_CHILD_ENTRY_NEXT(DATA_NCT_GET()->last_entry) = new_node;
    }

    DATA_NCT_GET()->last_entry = new_node;
    HTinsert(DATA_NCT_GET()->added_refs, reference, (void *) 1);
}

static void add_inheritance_entry_list(node_st *nodeset) {
    for (node_st *entry = INODESET_CHILDREN_TABLE(nodeset); entry != NULL;
         entry = NODESET_CHILD_ENTRY_NEXT(entry)) {
        add_inheritance_entry(NODESET_CHILD_ENTRY_REFERENCE(entry));
    }
}

/**
 * @fn NCTast
 */
node_st *NCTast(node_st *node) {
    DATA_NCT_GET()->symboltable = AST_STABLE(node);
    TRAVinodesets(node);
    return node;
}

/**
 * @fn NCTinodeset
 */
node_st *NCTinodeset(node_st *node) {
    // Are we called from within a setexpr or directly from ast?
    bool is_root = DATA_NCT_GET()->curr_set == NULL;

    // Is inheritance table already build?
    enum ht_state is_built = HTNCT_lookup(DATA_NCT_GET()->built_tables, node);
    if (is_built != ht_null) {
        // Detect recursion in setexpr
        if (is_built == ht_working) {
            struct ctinfo info;
            id_to_info(INODESET_NAME(node), &info);
            CTIobj(CTI_ERROR, true, info, "Recursion in setexpr\n");
            CCNerrorAction();
            return node;
        }

        // Add nodes to list if called within setexpr
        if (!is_root) {
            add_inheritance_entry_list(node);
        }
    } else {
        HTNCT_insert(DATA_NCT_GET()->built_tables, node, ht_working);
        if (is_root) { // No previous travdata to worry about
            // Set data
            DATA_NCT_GET()->curr_set = node;
            DATA_NCT_GET()->added_refs = HTnew_Ptr(htable_size);
            DATA_NCT_GET()->last_entry = NULL;

            TRAVexpr(node);

            // Clear data
            DATA_NCT_GET()->curr_set = NULL;
            HTdelete(DATA_NCT_GET()->added_refs);
            DATA_NCT_GET()->added_refs = NULL;
            DATA_NCT_GET()->last_entry = NULL;
        } else { // Restore old travdata when finished
            // Backup data
            node_st *prev = DATA_NCT_GET()->curr_set;
            htable_st *prev_refs = DATA_NCT_GET()->added_refs;
            node_st *prev_entry = DATA_NCT_GET()->last_entry;
            // Set data
            DATA_NCT_GET()->curr_set = node;
            DATA_NCT_GET()->added_refs = HTnew_Ptr(htable_size);
            DATA_NCT_GET()->last_entry = NULL;

            TRAVexpr(node);

            // Restore data
            DATA_NCT_GET()->curr_set = prev;
            HTdelete(DATA_NCT_GET()->added_refs);
            DATA_NCT_GET()->added_refs = prev_refs;
            DATA_NCT_GET()->last_entry = prev_entry;

            // Add nodesets to parent nodeset
            add_inheritance_entry_list(node);
        }
    }

    HTNCT_insert(DATA_NCT_GET()->built_tables, node, ht_finished);

    if (is_root) {
        TRAVnext(node);
    }

    return node;
}

/**
 * @fn NCTsetoperation
 */
node_st *NCTsetoperation(node_st *node) {
    if (SETOPERATION_TYPE(node) == SO_intersect ||
        SETOPERATION_TYPE(node) == SO_difference) {
        INODESET_ILLEGAL_SETEXPR_ATTR(DATA_NCT_GET()->curr_set) = true;
    }

    TRAVchildren(node);
    return node;
}

/**
 * @fn NCTsetliteral
 */
node_st *NCTsetliteral(node_st *node) {
    TRAVchildren(node);
    return node;
}

/**
 * @fn NCTsetreference
 */
node_st *NCTsetreference(node_st *node) {
    node_st *reference =
        STlookup(DATA_NCT_GET()->symboltable, SETREFERENCE_REFERENCE(node));
    assert(reference && NODE_TYPE(reference) == NT_INODESET);
    TRAVdo(reference);
    add_inheritance_entry(reference);
    return node;
}
