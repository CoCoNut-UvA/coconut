// Hash: (null)
#include "ccn/actions_core.h"
#include "ccn/ast_core.h"

Node *pass_createast(Node *arg_node) {
    Node *num = node_init_num(10);
    Node *root = node_init_root(num);
    return root;
}
