#include <stddef.h>
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "ccn/phase_driver.h"

int main() {
    node_st *num = ASTnewNum(10);
    node_st *num2 = ASTnewNum(100);
    node_st *binop = ASTnewBinOp(num, num2);
    
    CCNstart(binop);

    return 0;    
}

node_st *travbinop(node_st *binop) {
    TRAVchildren(binop);
    return binop;
}

node_st *travnum(node_st *num) {
    printf("%d\n", NUM_VALUE(num));
    return num;
}

node_st *p(node_st *tree) {
    printf("Tree: %p\n", tree);
    return tree;
}


node_st *changebinop(node_st *binop) {
    node_st *new = ASTnewBinOp(BINOP_LEFT(binop), NULL);
    TRAVchildren(binop);
    BINOP_RIGHT(new) = binop;
    return new;
}

node_st *changenum(node_st *num) {
    return ASTnewNum(1000 * NUM_VALUE(num));
    return num;
}

