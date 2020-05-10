#include <stdio.h>
#include <string.h>

#include "include/core/ast_core.h"
#include "include/core/trav_core.h"
#include "lib/memory.h"

#include "include/core/copy_core.h"

Node *copy_node(Node *node) { trav_start(node, TRAV_copy); }

char *copy_string(char *str) {
    if (str != NULL) {
        str = mem_copy(str, sizeof(char) * (strlen(str) + 1));
    }
    return str;
}