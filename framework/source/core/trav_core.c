#include <stdio.h>

#include "include/core/trav_core.h"
#include "include/core/vtables_core.h"
#include "include/generated/enum.h"
#include "lib/memory.h"
#include "lib/print.h"

Trav *trav_init(void) {
    Trav *trav = (Trav *)mem_alloc(sizeof(Trav));
    return trav;
}

Trav *trav_init_error(void) {
    print_user_error("traversal-driver",
                     "Trying to init data of unknown traversal.");
    return NULL;
}

void trav_free_error(Trav *trav) {
    (void)trav;
    print_user_error("traversal-driver",
                     "Trying to free data of unknown traversal.");
}

void trav_free(Trav *trav) { mem_free(trav); }

void trav_push(TravType trav_type) {
    InitFunc init_func = trav_data_init_vtable[trav_type];
    Trav *ts = init_func();
    ts->trav_type = trav_type;
    ts->prev = current_traversal;
    current_traversal = ts;
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
    current_traversal = prev;
}

Trav *trav_current(void) { return current_traversal; }

/* Start new traversal and push it to the traversal stack */
Node *trav_start(Node *syntaxtree, TravType trav_type) {
    trav_push(trav_type);
    syntaxtree = trav(syntaxtree);
    trav_pop();
    return syntaxtree;
}

/* Main traverse function, to be called by the user */
Node *trav(Node *arg_node) {
    TravFunc trav_func = trav_mat[TRAV_TYPE][NODE_TYPE(arg_node)];
    return trav_func(arg_node);
}

/* Optional traverse function, to be called by the user. Adds a null check to
 * signify optional child nodes */
Node *trav_opt(Node *arg_node) {
    if (!arg_node) {
        return arg_node;
    }
    return trav(arg_node);
}

/* Don't traverse through children and return */
Node *trav_return(Node *arg_node) { return arg_node; }

/* Traverse through each child node and return */
Node *trav_children(Node *arg_node) {
    if (NODE_CHILDREN(arg_node)) {
        for (int i = 0; i < NODE_NUMCHILDREN(arg_node); i++) {
            NODE_CHILDREN(arg_node)[i] = trav_opt(NODE_CHILDREN(arg_node)[i]);
        }
    }
    return arg_node;
}

Node *trav_error(Node *arg_node) {
    print_user_error("traversal-driver",
                     "Trying to traverse through node of unknown type.");
    return arg_node;
}