#include <stdio.h>

#include "generated/ast.h"
#include "generated/trav.h"
#include "lib/memory.h"

#include "core/free_core.h"

static Info *free_make_info() {
    Info *result;

    result = mem_alloc(sizeof(Info));

    INFO_FREE_FLAG(result) = NULL;
    INFO_FREE_ASSIGN(result) = NULL;

    return result;
}

static Info *free_info(Info *info) {
    info = mem_free(info);

    return info;
}

Node *free_node(Node *syntaxtree) {
    Info *arg_info = free_make_info();

    INFO_FREE_FLAG(arg_info) = syntaxtree;

    TRAVpush(TRAV_free);

    syntaxtree = traverse(syntaxtree, arg_info);

    TRAVpop();

    arg_info = free_info(arg_info);

    return syntaxtree;
}

Node *free_tree(Node *syntaxtree) {
    Info *arg_info = free_make_info();

    INFO_FREE_FLAG(arg_info) = NULL;

    TRAVpush(TRAV_free);

    syntaxtree = traverse(syntaxtree, arg_info);

    TRAVpop();

    arg_info = free_info(arg_info);

    return syntaxtree;
}

char *free_string(char *str) {

    if (str != NULL) {
        str = mem_free(str);
    }

    return str;
}