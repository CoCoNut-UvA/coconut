/**
 * @file
 *
 * Generate the base node, which contains node type and integers for locations
 * and generate the union containing all nodes.
 */

#include <stdio.h>
#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "globals.h"


static char *basic_node_type = "node_st";

node_st *dynamicGenBaseNodeInit(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_START_FUNC("%s *NewNode()", basic_node_type);
    OUT_FIELD("%s *node = MEMmalloc(sizeof(%s))", basic_node_type, basic_node_type);
    OUT_FIELD("NODE_TYPE(node) = NT_NULL");
    OUT_FIELD("NODE_CHILDREN(node) = NULL");
    OUT_FIELD("NODE_FILENAME(node) = NULL");
    OUT_FIELD("NODE_NUMCHILDREN(node) = 0");
    OUT_FIELD("NODE_BLINE(node) = 0");
    OUT_FIELD("NODE_ELINE(node) = 0");
    OUT_FIELD("NODE_BCOL(node) = 0");
    OUT_FIELD("NODE_ECOL(node) = 0");
    OUT_FIELD("return node");
    OUT_END_FUNC();

    return root;
}


node_st *dynamicGenBaseNode(node_st *root)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("#define NODE_TYPE(n) ((n)->nodetype)\n");
    OUT("#define NODE_CHILDREN(n) ((n)->children)\n");
    OUT("#define NODE_NUMCHILDREN(n) ((n)->num_children)\n");
    OUT("#define NODE_FILENAME(n) ((n)->filename)\n");
    OUT("#define NODE_BLINE(n) ((n)->begin_line)\n");
    OUT("#define NODE_ELINE(n) ((n)->end_line)\n");
    OUT("#define NODE_BCOL(n) ((n)->begin_col)\n");
    OUT("#define NODE_ECOL(n) ((n)->end_col)\n");
    OUT_TYPEDEF_STRUCT("ccn_node");
    OUT_FIELD("enum ccn_nodetype nodetype");
    OUT_FIELD("union NODE_DATA data");
    OUT_FIELD("struct ccn_node **children");
    OUT_FIELD("char *filename");
    OUT_FIELD("long int num_children");
    OUT_FIELD("uint32_t begin_line");
    OUT_FIELD("uint32_t end_line");
    OUT_FIELD("uint32_t begin_col");
    OUT_FIELD("uint32_t end_col");
    OUT_TYPEDEF_STRUCT_END("ccn_node");

    return root;
}

node_st *DGBUast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_UNION("NODE_DATA");
    TRAVchildren(node);
    OUT_STRUCT_END();
    return node;
}


node_st *DGBUinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    TRAVopt(INODE_NEXT(node));
    OUT_FIELD("struct NODE_DATA_%s *N_%s", ID_UPR((INODE_NAME(node))), ID_LWR(INODE_NAME(node)));
    return node;
}
