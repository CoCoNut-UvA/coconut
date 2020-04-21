#include "ast_core.h"
#include "../../../palm/include/lib/memory.h"

Node *node_init() {
    Node *node = (Node *)mem_alloc(sizeof(Node));
    return node;
}