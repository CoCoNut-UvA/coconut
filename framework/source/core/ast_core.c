#include "include/core/ast_core.h"
#include "lib/memory.h"

Node *node_init() {
    Node *node = (Node *)mem_alloc(sizeof(Node));
    NODE_TYPE(node) = NT_NULL;
    NODE_CHILDREN(node) = NULL;
    return node;
}