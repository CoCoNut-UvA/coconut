#include "ccn/dynamic_core.h"
#include "ccn/ccn_types.h"

#include "ccngen/ast.h"

#include "palm/memory.h"


typedef struct ccn_trav {
    struct ccn_trav *prev;
    enum ccn_traversal_type trav_type;
} ccn_trav_st;

static ccn_trav_st *current_traversal;

void TRAVpush(enum ccn_traversal_type trav_type) {
    ccn_trav_st *trav = MEMmalloc(sizeof(ccn_trav_st));
    trav->trav_type = trav_type;
    trav->prev = current_traversal;
    current_traversal = trav;

    // TODO: handle trav data.
    //InitFunc init_func = trav_data_init_vtable[trav_type];
    //init_func(trav);
}

void TRAVpop() {
    if (current_traversal == NULL) {
        // TODO: throw an error?
        return;
    }
    ccn_trav_st *prev = current_traversal->prev;

    //FreeFunc free_func = trav_data_free_vtable[TRAV_TYPE];
    //free_func(current_traversal);

    MEMfree(current_traversal);
    current_traversal = prev;
}

ccn_trav_st *TRAVgetCurrent(void) { return current_traversal; }

/* Main traverse function, to be called by the user */
struct ccn_node *TRAVdo(struct ccn_node *arg_node) {
    //assert(arg_node != NULL);
    ccn_trav_ft trav_func = ccn_trav_vtable[current_traversal->trav_type][NODE_TYPE(arg_node)];
    return trav_func(arg_node);
}

/* Start new traversal and push it to the traversal stack */
struct ccn_node *TRAVstart(struct ccn_node *syntaxtree, enum ccn_traversal_type trav_type) {
    TRAVpush(trav_type);
    syntaxtree = TRAVdo(syntaxtree);
    TRAVpop();
    return syntaxtree;
}

/* Optional traverse function, to be called by the user. Adds a null check to
 * signify optional child nodes */
struct ccn_node *TRAVopt(struct ccn_node *arg_node) {
    if (arg_node != NULL) {
        return TRAVdo(arg_node);
    }
    return arg_node;
}

/* Don't traverse through children and return */
struct ccn_node *TRAVnop(struct ccn_node *arg_node) { return arg_node; }

/* ccn_trav_sterse through each child node and return */
struct ccn_node *TRAVchildren(struct ccn_node *arg_node) {
    for (int i = 0; i < NODE_NUMCHILDREN(arg_node); i++) {
        NODE_CHILDREN(arg_node)[i] = TRAVopt(NODE_CHILDREN(arg_node)[i]);
    }
    return arg_node;
}

struct ccn_node *TRAVerror(struct ccn_node *arg_node) {
    //"Trying to traverse through node of unknown type.");
    abort();
    return arg_node;
}

struct ccn_node *PASSstart(struct ccn_node *syntaxtree, enum ccn_pass_type pass_type)
{
    return ccn_pass_vtable[pass_type](syntaxtree);
}

struct ccn_node *PASSerror(struct ccn_node *arg_node) {
    abort();
}

//struct ccn_node *node_copy(struct ccn_node *arg_node) { return trav_start(arg_node, TRAV_copy); }
//struct ccn_node *node_free(struct ccn_node *arg_node) { return trav_start(arg_node, TRAV_free); }