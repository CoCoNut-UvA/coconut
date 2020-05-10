#include <stdio.h>

#include "include/core/ast_core.h"
#include "include/core/trav_core.h"
#include "lib/memory.h"

#include "include/core/free_core.h"

Node *free_node(Node *node) { trav_start(node, TRAV_free); }