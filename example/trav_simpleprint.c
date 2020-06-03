// Hash: (null)
#include "ccn/trav_core.h"
#include "ccn/ast_core.h"
#include "lib/memory.h"

Trav *simpleprint_init_trav_data(Trav *trav) {
    // Define data here
    return trav;
}

void simpleprint_free_trav_data(Trav *trav) {
    // Free attributes here
}

Node *simpleprint_root(Node *arg_node) {
    arg_node = trav_children(arg_node);
    return arg_node;
}

Node *simpleprint_exprlist(Node *arg_node) {
    arg_node = trav_children(arg_node);
    return arg_node;
}

Node *simpleprint_unop(Node *arg_node) {
    arg_node = trav_children(arg_node);
    return arg_node;
}

Node *simpleprint_binop(Node *arg_node) {
    arg_node = trav_children(arg_node);
    return arg_node;
}

Node *simpleprint_var(Node *arg_node) {
    return arg_node;
}

Node *simpleprint_num(Node *arg_node) {
    printf("%d\n", NUM_VALUE(arg_node));
    return arg_node;
}


