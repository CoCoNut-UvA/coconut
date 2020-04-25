#include <stdio.h>

#include "core/ast_core.h"
#include "core/trav_core.h"
#include "lib/memory.h"

#include "core/free_core.h"

struct TRAV_DATA {};

TravData *free_init_data() {
    TravData *data = mem_alloc(sizeof(TravData));
    return data;
}

void free_free_data(TravData *data) { mem_free(data); }

Node *free_node(Node *node) {
    trav_start(node, TRAV_free, &free_init_data, &free_free_data);
}