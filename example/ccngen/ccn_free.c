#include <stdlib.h>
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "palm/memory.h"

struct ccn_node *DELbinop(struct ccn_node *arg_node) {
    arg_node = TRAVchildren(arg_node);
    MEMfree(arg_node->data.N_binop);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

struct ccn_node *DELnum(struct ccn_node *arg_node) {
    MEMfree(arg_node->data.N_num);
    arg_node = MEMfree(arg_node);
    return arg_node;
}

