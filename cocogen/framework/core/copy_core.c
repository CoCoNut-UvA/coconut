#include <stdio.h>
#include <string.h>

#include "core/ast_core.h"
#include "core/copy_core.h"
#include "core/trav_core.h"
#include "generated/globaldata.h"
#include "lib/memory.h"

typedef struct COPY_DATA {
} CopyData;

CopyData *copy_init_data() {
    CopyData *result;
    result = mem_alloc(sizeof(CopyData));
    return result;
}

void copy_free_data(CopyData *data) { mem_free(data); }

Node *copy_node(Node *syntaxtree) {
    CopyData *arg_data = copy_init_data();
    trav_push(TRAV_copy);
    syntaxtree = traverse(syntaxtree);
    trav_pop();
    copy_free_data(arg_data);
    return syntaxtree;
}

char *copy_string(char *str) {
    if (str != NULL) {
        str = mem_copy(str, sizeof(char) * (strlen(str) + 1));
    }
    return str;
}