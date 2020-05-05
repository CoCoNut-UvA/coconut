#include <stdio.h>

#include "core/trav_core.h"
#include "generated/enum.h"
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

void trav_push(TravType travtype) {
    Trav *ts = trav_data_init_array[travtype]();
    ts->travtype = travtype;
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
    trav_data_free_array[TRAV_TYPE](current_traversal);
    current_traversal = prev;
}

Trav *trav_current(void) { return current_traversal; }

Node *trav_start(Node *syntaxtree, TravType travtype) {
    trav_push(travtype);
    syntaxtree = traverse(syntaxtree);
    trav_pop();
    return syntaxtree;
}

Node *trav_pass(Node *arg_node) {
    if (!arg_node) {
        return arg_node;
    }
    return NODE_TRAV(arg_node);
}

Node *trav_node(Node *arg_node) {
    if (!arg_node) {
        print_user_error("traversal-driver", "Mandatory subtree is NULL");
        return arg_node;
    }
    return NODE_TRAV(arg_node);
}

Node *traverse(Node *arg_node) {
    if (!arg_node) {
        return arg_node;
    }
    return trav_mat[TRAV_TYPE][NODE_TYPE(arg_node)](arg_node);
}

Node *trav_noop(Node *arg_node) { return arg_node; }
Node *trav_error(Node *arg_node) {
    print_user_error("traversal-driver",
                     "Trying to traverse through node of unknown type.");
    return arg_node;
}