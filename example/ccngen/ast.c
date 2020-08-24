#include "ccngen/ast.h"
#include "palm/memory.h"
node_st *NewNode() {
    node_st *node = MEMmalloc(sizeof(node_st));
    NODE_TYPE(node) = NT_NULL;
    NODE_CHILDREN(node) = NULL;
    NODE_NUMCHILDREN(node) = 0;
    return node;
}

node_st *ASTnewBinOp(node_st *left, node_st *right) {
    node_st *node = NewNode();
    node->data.N_binop = MEMmalloc(sizeof(struct NODE_DATA_BINOP));
    NODE_TYPE(node) = NT_BINOP;
    NODE_CHILDREN(node) = node->data.N_binop->binop_children.binop_children_at;
    NODE_NUMCHILDREN(node) = 2;
    BINOP_LEFT(node) = left;
    BINOP_RIGHT(node) = right;
    return node;
}

node_st *ASTnewNum(int value) {
    node_st *node = NewNode();
    node->data.N_num = MEMmalloc(sizeof(struct NODE_DATA_NUM));
    NODE_TYPE(node) = NT_NUM;
    NODE_CHILDREN(node) = node->data.N_num->num_children.num_children_at;
    NODE_NUMCHILDREN(node) = 0;
    NUM_VALUE(node) = value;
    return node;
}

