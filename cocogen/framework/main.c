#include <stdio.h>

#include "core/ast_core.h"
#include "core/trav_core.h"
#include "generated/ast.h"
#include "generated/enum.h"
#include "generated/globaldata.h"
#include "lib/array.h"

int main(int argc, char *argv[]) {
    init_globaldata();
    Node *left = node_init_intconst(4);
    Node *right = node_init_intconst(5);
    Node *binop = node_init_binop(left, right, BO_add);
    print_start(binop);
    free_node(binop);
}