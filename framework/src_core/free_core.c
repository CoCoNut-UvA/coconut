#include <stdio.h>

#include "inc_core/ast_core.h"
#include "inc_core/trav_core.h"
#include "lib/memory.h"

#include "inc_core/free_core.h"

Node *free_node(Node *node) { trav_start(node, TRAV_free); }