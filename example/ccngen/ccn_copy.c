#include <stdlib.h>
#include "ccn/dynamic_core.h"
#include "ccn/ccn_types.h"
#include "ccngen/ast.h"
#include "palm/str.h"

struct ccn_node *CPYbinop(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTnewBinOp(NULL, NULL    );
    arg_node = TRAVchildren(arg_node);
    return new_node;
}

struct ccn_node *CPYnum(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTnewNum(0    );
    NUM_VALUE(new_node) = NUM_VALUE(arg_node);
    return new_node;
}

