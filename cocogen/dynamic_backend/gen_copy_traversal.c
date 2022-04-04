/**
 * @file
 *
 * This generates the copy traversal to be used in a compiler.
 * Copying of children is deep, copying of attributes is deep on
 * primitives types. Attribute nodes are shallow copied.
 */

#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "dynamic_backend/gen_helpers.h"

static int arg_num = 0;
static node_st *curr_node;

node_st *DGCTast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "ccn_copy.c");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include <stddef.h>\n");
    OUT("#include \"palm/str.h\"\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT_START_FUNC("void CopyBaseNode(node_st *target, node_st *source)");
    OUT_STATEMENT("NODE_BCOL(target) = NODE_BCOL(source)");
    OUT_STATEMENT("NODE_ECOL(target) = NODE_ECOL(source)");
    OUT_STATEMENT("NODE_BLINE(target) = NODE_BLINE(source)");
    OUT_STATEMENT("NODE_ELINE(target) = NODE_ELINE(source)");
    OUT_STATEMENT("NODE_FILENAME(target) = STRcpy(NODE_FILENAME(source))");
    OUT_END_FUNC();
    TRAVopt(AST_INODES(node));
    return node;
}

node_st *DGCTiactions(node_st *node)
{
    return node;
}

node_st *DGCTiphase(node_st *node)
{
    return node;
}

node_st *DGCTitraversal(node_st *node)
{
    return node;
}

node_st *DGCTitravdata(node_st *node)
{
    return node;
}

node_st *DGCTipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGCTinode(node_st *node)
{
    const char *node_argument_name = "arg_node";
    const char *new_node_name = "new_node";
    GeneratorContext *ctx = globals.gen_ctx;
    curr_node = node;
    arg_num = 0;
    OUT_START_FUNC(DGH_TRAV_FUNC_SIG(), "CPY", DGH_TRAVERSAL_TARGET_ID(INODE_NAME(node)), node_argument_name);
    TRAVstart(node, TRAV_DGCC);
    OUT_STATEMENT("CopyBaseNode(%s, %s)", new_node_name, node_argument_name);
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    OUT_FIELD("return %s", new_node_name);
    OUT_END_FUNC();
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *DGCTinodeset(node_st *node)
{
    return node;
}

node_st *DGCTchild(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *node_name = ID_UPR(INODE_NAME(curr_node));
    char *child_name = ID_UPR(CHILD_NAME(node));
    OUT_FIELD("%s_%s(new_node) = TRAVopt(%s_%s(arg_node))", node_name, child_name, node_name, child_name);
    TRAVopt(CHILD_NEXT(node));
    return node;
}

node_st *DGCTattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *node_name = ID_UPR(INODE_NAME(curr_node));
    char *attr_name = ID_UPR(ATTRIBUTE_NAME(node));
    if (ATTRIBUTE_TYPE(node) == AT_string) {
        OUT_FIELD("%s_%s(new_node) = STRcpy(%s_%s(arg_node))", node_name, attr_name, node_name, attr_name);
    } else {
        OUT_FIELD("%s_%s(new_node) = %s_%s(arg_node)", node_name, attr_name, node_name, attr_name);
    }
    TRAVopt(ATTRIBUTE_NEXT(node));
    return node;
}

node_st *DGCTste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGCTienum(node_st *node)
{
    return node;
}

node_st *DGCTid(node_st *node)
{
    return node;
}

node_st *DGCTilifetime(node_st *node)
{
    return node;
}

node_st *DGCTlifetime_range(node_st *node)
{
    return node;
}
