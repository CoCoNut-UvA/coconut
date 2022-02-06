#include "ccn/dynamic_core.h"
#include "ccn/ccn_types.h"
#include <err.h>
#include <stdio.h>

#include "ccngen/ast.h"

#include "palm/memory.h"


void TRAVdataNOP(ccn_trav_st *trav)
{
    trav = (void*)trav;
    return;
}

static ccn_trav_st *current_traversal;

void TRAVpush(enum ccn_traversal_type trav_type) {
    ccn_trav_st *trav = MEMmalloc(sizeof(ccn_trav_st));
    trav->trav_type = trav_type;
    trav->prev = current_traversal;
    current_traversal = trav;

    ccn_trav_data_ft init_func = trav_data_init_vtable[trav_type];
    init_func(trav);
}

void TRAVpop() {
    if (current_traversal == NULL) {
        err(EXIT_FAILURE, "[coconut] Error in framework.");
    }
    ccn_trav_st *prev = current_traversal->prev;

    ccn_trav_data_ft free_func = trav_data_free_vtable[current_traversal->trav_type];
    free_func(current_traversal);

    MEMfree(current_traversal);
    current_traversal = prev;
}

ccn_trav_st *TRAVgetCurrent(void) { return current_traversal; }

/**
 * Traverse the supplied node.
 * This function requires that arg_node != NULL
 * */
struct ccn_node *TRAVdo(struct ccn_node *arg_node) {
    //assert(arg_node != NULL);
    ccn_trav_ft trav_func = ccn_trav_vtable[current_traversal->trav_type][NODE_TYPE(arg_node)];
    return trav_func(arg_node);
}

/* Start new traversal and push it to the traversal stack */
struct ccn_node *TRAVstart(struct ccn_node *syntaxtree, enum ccn_traversal_type trav_type) {
    TRAVpush(trav_type);
    syntaxtree = TRAVopt(syntaxtree);
    TRAVpop();
    return syntaxtree;
}

/** Optional traverse function.
 * Adds a null check to signify optional child nodes
 */
struct ccn_node *TRAVopt(struct ccn_node *arg_node) {
    if (arg_node != NULL) {
        return TRAVdo(arg_node);
    }
    return arg_node;
}

/* Don't traverse through children and return */
struct ccn_node *TRAVnop(struct ccn_node *arg_node) { return arg_node; }

/**
 * Helper function that traverses all children in a node. 
 * This function assigns the result of the traversals to it respective child.
 * The order is determined by the order in the specification file.
 */
struct ccn_node *TRAVchildren(struct ccn_node *arg_node) {
    for (int i = 0; i < NODE_NUMCHILDREN(arg_node); i++) {
        NODE_CHILDREN(arg_node)[i] = TRAVopt(NODE_CHILDREN(arg_node)[i]);
    }
    return arg_node;
}

struct ccn_node *TRAVerror(struct ccn_node *arg_node) {
    //"Trying to traverse through node of unknown type.");
    fprintf(stderr, "Traversing trough unkown node, is the node corrupted?");
    abort();
    return arg_node;
}

struct ccn_node *PASSstart(struct ccn_node *syntaxtree, enum ccn_pass_type pass_type)
{
    return ccn_pass_vtable[pass_type](syntaxtree);
}

struct ccn_node *PASSerror(struct ccn_node *arg_node) {
    (void)arg_node;
    abort();
}

/**
 * Makes a deep copy of the given node.
 */
struct ccn_node *CCNcopy(struct ccn_node *arg_node) { return TRAVstart(arg_node, TRAV_cpy); }

/**
 * Free a node, all its children, and allocated attributes.
 */
struct ccn_node *CCNfree(struct ccn_node *arg_node) { return TRAVstart(arg_node, TRAV_free); }
