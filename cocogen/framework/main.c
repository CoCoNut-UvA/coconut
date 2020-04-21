#include <stdio.h>

#include "core/ast_core.h"

#include "core/ast_core.h"
#include "core/trav_core.h"
#include "generated/ast.h"
#include "generated/enum.h"
#include "generated/trav.h"

typedef struct INFO {
} Info;

int main(int argc, char *argv[]) {
    Info *info;
    Node *left = node_init_intconst(4);
    Node *right = node_init_intconst(5);
    Node *binop = node_init_binop(left, right, BO_add);
    trav_push(TRAV_print);
    print_binop(binop, info);
    trav_pop();
}