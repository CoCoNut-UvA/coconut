// Hash: (null)
#include "ccn/ast_core.h"
#include "ccn/trav_core.h"
#include "lib/memory.h"
#include <stdio.h>

Trav *simpleprint_init_trav_data(Trav *trav) {
    SIMPLEPRINT_NODECOUNT = 1;
    return trav;
}

void simpleprint_free_trav_data(Trav *trav) {
    // Free attributes here
}

Node *simpleprint_root(Node *arg_node) {
    printf("root\n");
    arg_node = trav_children(arg_node);
    return arg_node;
}

Node *simpleprint_exprlist(Node *arg_node) {
    printf("expr\n");
    arg_node = trav_children(arg_node);
    return arg_node;
}

Node *simpleprint_unop(Node *arg_node) {
    printf("unop op=%d\n", UNOP_OP(arg_node));
    arg_node = trav_children(arg_node);
    return arg_node;
}

Node *simpleprint_binop(Node *arg_node) {
    printf("binop op=%d\n", BINOP_OP(arg_node));
    arg_node = trav_children(arg_node);
    return arg_node;
}

Node *simpleprint_var(Node *arg_node) {
    printf("var id=%s\n", VAR_ID(arg_node));
    return arg_node;
}

Node *simpleprint_num(Node *arg_node) {
    printf("num value=%d\n", NUM_VALUE(arg_node));
    return arg_node;
}
