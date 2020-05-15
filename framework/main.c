#include <stdio.h>

#include "include/core/actions_core.h"
#include "include/core/ast_core.h"
#include "include/core/trav_core.h"
#include "include/core/vtables_core.h"

int main(int argc, char *argv[]) {
    Node *left = node_init_intconst(4);
    Node *right = node_init_intconst(5);
    Node *binop = node_init_binop(left, right, BO_add);
    Node *expr = node_init_exprlist(binop, NULL);
    trav_start(expr, TRAV_print);
    trav_start(expr, TRAV_context);
    pass_start(expr, PASS_scanparse);
    node_free(expr);
}