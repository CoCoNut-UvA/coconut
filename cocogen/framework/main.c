#include <stdio.h>

#include "core/ast_core.h"

#include "core/ast_core.h"
#include "generated/ast.h"
#include "generated/enum.h"

int main(int argc, char *argv[]) {
    Node *left = node_init_intconst(4);
    Node *right = node_init_intconst(5);
    Node *binop = node_init_binop(left, right, BO_add);
    printf("leftvalue: %d\n", INTCONST_VALUE(BINOP_LEFT(binop)));
    printf("rightvalue: %d\n", INTCONST_VALUE(BINOP_RIGHT(binop)));
    printf("binop: %d\n", BINOP_OP(binop));
}