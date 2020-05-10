#include <stdio.h>

#include "inc_core/trav_core.h"
#include "inc_core/vtables_core.h"
#include "inc_generated/enum.h"
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

Node *trav_start(Node *syntaxtree, TravType trav_type) {
    trav_push(trav_type);
    syntaxtree = traverse(syntaxtree);
    trav_pop();
    return syntaxtree;
}

Node *trav_node(Node *arg_node) {
    if (!arg_node) {
        print_user_error("traversal-driver", "Mandatory subtree is NULL");
        return arg_node;
    }
    TravFunc trav_func = trav_mat[TRAV_TYPE][NODE_TYPE(arg_node)];
    return trav_func(arg_node);
}

Node *traverse(Node *arg_node) {
    if (!arg_node) {
        return arg_node;
    }
    TravFunc trav_func = trav_mat[TRAV_TYPE][NODE_TYPE(arg_node)];
    return trav_func(arg_node);
}

Node *trav_noop(Node *arg_node) { return arg_node; }
Node *trav_error(Node *arg_node) {
    print_user_error("traversal-driver",
                     "Trying to traverse through node of unknown type.");
    return arg_node;
}