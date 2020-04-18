#include "ast_core.h"
#include "lib/memory.h"

static Node *node_init() {
    Node *node = (Node *)mem_alloc(sizeof(Node));

    // TODO: make this work
    // NODE_LINE(node) = global.line;
    // NODE_COL(node) = global.col;

    return node;
}