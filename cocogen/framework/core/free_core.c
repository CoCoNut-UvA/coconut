#include <stdio.h>

#include "core/free_core.h"
#include "core/trav_core.h"
#include "generated/ast.h"
#include "lib/memory.h"

struct TRAV_DATA {};

TraversalData *free_init_data() {
    TraversalData *data = mem_alloc(sizeof(TraversalData));
    return data;
}

void free_free_data(TraversalData *data) { mem_free(data); }

Node *free_node(Node *node) {
    trav_start(node, TRAV_free, &free_init_data, &free_free_data);
}