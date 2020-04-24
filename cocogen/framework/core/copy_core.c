#include <stdio.h>
#include <string.h>

#include "core/ast_core.h"
#include "core/trav_core.h"
#include "lib/memory.h"

#include "core/copy_core.h"

struct TRAV_DATA {};

TraversalData *copy_init_data() {
    TraversalData *data = mem_alloc(sizeof(TraversalData));
    return data;
}

void copy_free_data(TraversalData *data) { mem_free(data); }

Node *copy_node(Node *node) {
    trav_start(node, TRAV_copy, &copy_init_data, &copy_free_data);
}

char *copy_string(char *str) {
    if (str != NULL) {
        str = mem_copy(str, sizeof(char) * (strlen(str) + 1));
    }
    return str;
}