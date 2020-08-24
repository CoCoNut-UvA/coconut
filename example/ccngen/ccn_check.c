#include <stdlib.h>
#include <stdbool.h>

#include "ccn/dynamic_core.h"
#include "ccngen/ast.h"
#include "palm/memory.h"
#include "palm/ctinfo.h"

static bool NodeIsInConstants(struct ccn_node *arg_node) {
    return (NODE_TYPE(arg_node) == NT_NUM || false);
}

static bool NodeIsInExpr(struct ccn_node *arg_node) {
    return (NODE_TYPE(arg_node) == NT_BINOP || NODE_TYPE(arg_node) == NT_NUM || false);
}

struct ccn_node *CHKbinop(struct ccn_node *arg_node) {
    if (BINOP_LEFT(arg_node)) {
        if (!NodeIsInExpr(BINOP_LEFT(arg_node))) {
            CTIerror("Inconsistency found!");
        }

    }

    if (BINOP_RIGHT(arg_node)) {
        if (!NodeIsInExpr(BINOP_RIGHT(arg_node))) {
            CTIerror("Inconsistency found!");
        }

    }

    arg_node = TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKnum(struct ccn_node *arg_node) {
    return arg_node;
}

