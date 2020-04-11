#include "generated/ast.h"
#include "lib/memory.h"

static Node *node_init_empty() {
    Node *node = (Node *)mem_alloc(sizeof(Node));

    // TODO: make this work
    // NODE_LINE(node) = global.line;
    // NODE_COL(node) = global.col;

    return node;
}