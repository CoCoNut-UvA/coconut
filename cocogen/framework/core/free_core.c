#include "core/free_core.h"
#include "generated/ast.h"
#include <stdio.h>

static Info *make_info() {
    Info *result;

    result = mem_alloc(sizeof(Info));

    INFO_FREE_FLAG(result) = NULL;
    INFO_FREE_ASSIGN(result) = NULL;

    return result;
}

static Info *free_info(Info *info) {
    info = MEMfree(info);

    return info;
}

Node *free_node(Node *syntaxtree) {
    Info *arg_info = make_info();

    INFO_FREE_FLAG(arg_info) = syntaxtree;

    TRAVpush(TRAV_free);

    syntaxtree = TRAVdo(syntaxtree, arg_info);

    TRAVpop();

    arg_info = free_info(arg_info);

    return syntaxtree;
}

Node *free_tree(Node *syntaxtree) {
    Info *arg_info = make_info();

    INFO_FREE_FLAG(arg_info) = NULL;

    TRAVpush(TRAV_free);

    syntaxtree = TRAVdo(syntaxtree, arg_info);

    TRAVpop();

    arg_info = free_info(arg_info);

    return syntaxtree;
}
