// Hash: (null)
#include "ccn/actions_core.h"
#include "ccn/ast_core.h"
#include <stdlib.h>

Node *pass_createast(Node *arg_node) {
    Node *num1 = node_init_num(42);
    Node *num2 = node_init_num(69);
    Node *binop = node_init_binop(num1, num2, BO_add);
    Node *exprlist1 = node_init_exprlist(binop, NULL);
    Node *var = node_init_var("somevar");
    Node *exprlist2 = node_init_exprlist(var, exprlist1);
    Node *root = node_init_root(exprlist2);
    return root;
}
