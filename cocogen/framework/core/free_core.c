#include <stdio.h>

#include "core/ast_core.h"
#include "core/trav_core.h"
#include "lib/memory.h"

#include "core/free_core.h"

Node *free_node(Node *node) {
    trav_start(node, TRAV_free, &trav_init, &trav_free);
}