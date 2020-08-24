#include <stdio.h>

#include "ccn/ast_core.h"
#include "ccn/trav_core.h"
#include "ccn/vtables_core.h"
#include "lib/memory.h"
#include "lib/print.h"
#include <assert.h>

static Trav *current_traversal = NULL;

Trav *trav_init_return(Trav *trav) { return trav; }
void trav_free_return(Trav *trav) {}

Trav *trav_init_error(Trav *trav) {
    print_user_error("traversal-driver",
                     "Trying to init data of unknown traversal.");
    return trav;
}

void trav_free_error(Trav *trav) {
    (void)trav;
    print_user_error("traversal-driver",
                     "Trying to free data of unknown traversal.");
}

void trav_push(TraversalType trav_type) {
    Trav *trav = (Trav *)mem_alloc(sizeof(Trav));
    trav->trav_type = trav_type;
    trav->prev = current_traversal;
    current_traversal = trav;
    InitFunc init_func = trav_data_init_vtable[trav_type];
    init_func(trav);
}

void trav_pop() {
    if (current_traversal == NULL) {
        print_user_error("traversal-driver",
                         "Cannot pop of empty traversal stack.");
        return;
    }
    Trav *prev = current_traversal->prev;
    FreeFunc free_func = trav_data_free_vtable[TRAV_TYPE];
    free_func(current_traversal);
    mem_free(current_traversal);
    current_traversal = prev;
}

Trav *trav_current(void) { return current_traversal; }

/* Start new traversal and push it to the traversal stack */
Node *trav_start(Node *syntaxtree, TraversalType trav_type) {
    trav_push(trav_type);
    syntaxtree = trav(syntaxtree);
    trav_pop();
    return syntaxtree;
}

/* Main traverse function, to be called by the user */
Node *trav(Node *arg_node) {
    assert(arg_node != NULL);
    TravFunc trav_func = trav_vtable[TRAV_TYPE][NODE_TYPE(arg_node)];
    return trav_func(arg_node);
}

/* Optional traverse function, to be called by the user. Adds a null check to
 * signify optional child nodes */
Node *trav_opt(Node *arg_node) {
    if (arg_node != NULL) {
        return trav(arg_node);
    }
    return arg_node;
}

/* Don't traverse through children and return */
Node *trav_return(Node *arg_node) { return arg_node; }

/* Traverse through each child node and return */
Node *trav_children(Node *arg_node) {
    assert(arg_node != NULL);
    for (int i = 0; i < NODE_NUMCHILDREN(arg_node); i++) {
        NODE_CHILDREN(arg_node)[i] = trav_opt(NODE_CHILDREN(arg_node)[i]);
    }
    return arg_node;
}

Node *trav_error(Node *arg_node) {
    print_user_error("traversal-driver",
                     "Trying to traverse through node of unknown type.");
    return arg_node;
}

Node *node_copy(Node *arg_node) { return trav_start(arg_node, TRAV_copy); }
Node *node_free(Node *arg_node) { return trav_start(arg_node, TRAV_free); }