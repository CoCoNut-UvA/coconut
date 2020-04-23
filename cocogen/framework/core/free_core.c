#include <stdio.h>

#include "core/free_core.h"
#include "generated/ast.h"
#include "generated/globaldata.h"
#include "generated/trav.h"
#include "lib/memory.h"

typedef struct FREE_DATA {
} FreeData;

FreeData *free_init_data() {
    FreeData *result = mem_alloc(sizeof(FreeData));
    return result;
}

void free_free_data(FreeData *data) { mem_free(data); }

Node *free_node(Node *syntaxtree) {
    FreeData *arg_data = free_init_data();
    trav_push(TRAV_free);
    syntaxtree = traverse(syntaxtree);
    trav_pop();
    free_free_data(arg_data);
    return NULL;
}