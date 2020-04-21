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

static void free_info(Info *info) { mem_free(info); }

void free_node(Node *syntaxtree) {
    Info *arg_info = free_make_info();

    INFO_FREE_FLAG(arg_info) = syntaxtree;

    trav_push(TRAV_free);

    syntaxtree = traverse(syntaxtree, arg_info);

    trav_pop();

    free_info(arg_info);
}

void free_tree(Node *syntaxtree) {
    Info *arg_info = free_make_info();

    INFO_FREE_FLAG(arg_info) = NULL;

    trav_push(TRAV_free);

    syntaxtree = traverse(syntaxtree, arg_info);

    trav_pop();

    free_info(arg_info);
}

void free_string(char *str) {

    if (str != NULL) {
        mem_free(str);
    }
}