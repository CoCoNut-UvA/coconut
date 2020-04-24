#include <stdio.h>

#include "core/ast_core.h"
#include "core/copy_core.h"
#include "core/free_core.h"
#include "core/trav_core.h"
#include "generated/ast.h"
#include "generated/enum.h"
#include "lib/array.h"

int main(int argc, char *argv[]) {
    Node *left = node_init_intconst(4);
    Node *right = node_init_intconst(5);
    Node *binop = node_init_binop(left, right, BO_add);
    trav_start(binop, TRAV_print, &print_init_data, &print_free_data);
    free_node(binop);
}