/**
 * @file
 *
 * Helper traversal that generates a constructor call for a node.
 * Can be used when needing to generate a new node.
 */

#include <stdio.h>

#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "globals.h"
#include "../gen_helpers.h"
#include "dynamic_backend/gen_helpers.h"


static int arg_num = 0;
static node_st *ast;


node_st *DGCCast(node_st *node)
{
    ast = node;
    TRAVopt(AST_INODES(node));
    return node;
}

node_st *DGCCinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    arg_num = 0;
    OUT(GH_DEFAULT_NODE_TYPE() " *new_node =" DGH_NODE_CONSTR_CALL_SIG(), DGH_NODE_CONSTR_TARGET(node));
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    OUT_NO_INDENT(");\n");

    return node;
}

node_st *DGCCchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (CHILD_IN_CONSTRUCTOR(node)) {
        if (arg_num) {
            OUT(", ");
        }
        arg_num++;
        OUT("NULL");
    }
    TRAVopt(CHILD_NEXT(node));
    return node;
}

node_st *DGCCattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (ATTRIBUTE_IN_CONSTRUCTOR(node)) {
        if (arg_num) {
            OUT(", ");
        }
        arg_num++;
        OUT("%s", FMTattributeDefaultVal(ATTRIBUTE_TYPE(node)));
    }
    TRAVopt(ATTRIBUTE_NEXT(node));
    return node;
}
