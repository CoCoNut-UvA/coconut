#include <stdio.h>

#include "generated/trav_table.h"
#include "trav_core.h"

typedef struct traversalstack {
    struct traversalstack *next;
    trav_fun_p *funs;
    TraversalType prefix;
} TraversalStack;

static TraversalStack *travstack = NULL;

Node *traverse_node(Node *arg_node, Info *arg_info) {
    if (arg_node == NULL) {
        /// TODO: do error handling, or throw warning?
        return arg_node;
    }

    /// TODO: fix globals
    // global.line = NODE_LINE(arg_node);

    if (pretable[travstack->prefix] != NULL) {
        arg_node = pretable[travstack->prefix](arg_node, arg_info);
    }

    arg_node = (travstack->funs[NODE_TYPE(arg_node)])(arg_node, arg_info);

    if (posttable[travstack->prefix] != NULL) {
        arg_node = posttable[travstack->prefix](arg_node, arg_info);
    }

    return arg_node;
}

Node *traverse_noop(Node *arg_node, Info *arg_info) { return arg_node; }

void push_new_traversal(TraversalType prefix) {
    TraversalStack *new;

    new = mem_alloc(sizeof(TraversalStack));

    new->next = travstack;
    new->prefix = prefix;
    new->funs = travtable[prefix];

    travstack = new;
}

TraversalType pop_cur_traversal(void) {
    TraversalStack *tmp;
    TraversalType prefix;

    tmp = travstack;
    travstack = tmp->next;
    prefix = tmp->prefix;

    tmp = mem_free(tmp);

    return prefix;
}

const char *traversal_name(void) {
    const char *name;

    if (travstack == NULL) {
        name = "no_active_traversal";
    } else {
        name = travnames[travstack->prefix];
    }

    return name;
}

void traversal_set_pre_fun(TraversalType prefix, trav_fun_p prefun) {
    pretable[prefix] = prefun;
}

void traversal_set_post_fun(TraversalType prefix, trav_fun_p postfun) {
    posttable[prefix] = postfun;
}
