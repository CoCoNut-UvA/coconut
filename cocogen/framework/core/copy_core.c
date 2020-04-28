#include <stdio.h>
#include <string.h>

#include "core/ast_core.h"
#include "core/trav_core.h"
#include "lib/memory.h"

#include "core/copy_core.h"

Node *copy_node(Node *node) {
    trav_start(node, TRAV_copy, &noop_init, &noop_free);
}

char *copy_string(char *str) {
    if (str != NULL) {
        str = mem_copy(str, sizeof(char) * (strlen(str) + 1));
    }
    return str;
}