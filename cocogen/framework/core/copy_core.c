#include <stdio.h>
#include <string.h>

#include "core/ast_core.h"
#include "core/copy_core.h"
#include "core/trav_core.h"
#include "lib/memory.h"

typedef struct INFO {

} Info;

static Info *free_make_info() {
    Info *result;

    result = mem_alloc(sizeof(Info));

    return result;
}

static Info *copy_info(Info *info) {
    info = mem_copy(info, sizeof(Info));

    return info;
}

Node *copy_node(Node *syntaxtree) {
    Info *arg_info = free_make_info();

    trav_push(TRAV_copy);

    syntaxtree = traverse(syntaxtree, arg_info);

    trav_pop();

    arg_info = copy_info(arg_info);

    return syntaxtree;
}

Node *copy_tree(Node *syntaxtree) {
    Info *arg_info = free_make_info();

    trav_push(TRAV_copy);

    syntaxtree = traverse(syntaxtree, arg_info);

    trav_pop();

    arg_info = copy_info(arg_info);

    return syntaxtree;
}

char *copy_string(char *str) {

    if (str != NULL) {
        str = mem_copy(str, sizeof(char) * (strlen(str) + 1));
    }

    return str;
}