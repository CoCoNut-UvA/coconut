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


static FILE *fp;
static int indent = 1;
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
    fp = globals.fp;
    arg_num = 0;
    OUT("struct ccn_node *new_node = AST%s(", ID_ORIG(INODE_NAME(node)));
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    OUT_NO_INDENT(");\n");

    return node;
}

node_st *DGCCchild(node_st *node)
{
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
