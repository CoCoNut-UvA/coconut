#include "assert.h"
#include <globals.h>
#include <stddef.h>
#include <stdio.h>

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "ccngen/trav.h"


static node_st *ast;
static node_st *curr_node;

node_st *DGFTast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "ccn_free.c");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("#include \"palm/memory.h\"\n");
    ast = node;
    TRAVinodes(node);
    return node;
}

node_st *DGFTiactions(node_st *node)
{
    return node;
}

node_st *DGFTiphase(node_st *node)
{
    return node;
}

node_st *DGFTitraversal(node_st *node)
{
    return node;
}

node_st *DGFTitravdata(node_st *node)
{
    return node;
}

node_st *DGFTipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGFTinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    curr_node = node;
    OUT_START_FUNC("struct ccn_node *DEL%s(struct ccn_node *arg_node)", ID_LWR(INODE_NAME(node)));
    if (INODE_ICHILDREN(node)) {
        OUT_FIELD("TRAVchildren(arg_node)");
    }
    TRAViattributes(node);
    OUT_FIELD("MEMfree(NODE_FILENAME(arg_node))");
    OUT_FIELD("MEMfree(arg_node->data.N_%s)", ID_LWR(INODE_NAME(node)));
    OUT_FIELD("MEMfree(arg_node)");
    OUT_FIELD("return NULL");
    OUT_END_FUNC();
    TRAVnext(node);
    return node;
}

node_st *DGFTinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGFTnodeset_child_entry(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGFTchild(node_st *node)
{

    return node;
}

node_st *DGFTattribute(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (ATTRIBUTE_TYPE(node) == AT_string) {
        OUT_FIELD("MEMfree(arg_node->data.N_%s->%s)", ID_LWR(INODE_NAME(curr_node)), ID_LWR(ATTRIBUTE_NAME(node)));
    }
    TRAVnext(node);
    return node;
}

/**
 * @fn DGFTequation
 */
node_st *DGFTequation(node_st *node)
{
    CTI(CTI_WARN, true, "Not implemented DGFTequation");
    TRAVchildren(node);
    return node;
}

/**
 * @fn DGFTequation_dependency
 */
node_st *DGFTequation_dependency(node_st *node)
{
    CTI(CTI_WARN, true, "Not implemented DGFTequation_dependency");
    TRAVchildren(node);
    return node;
}

/**
 * @fn DGFTattribute_reference
 */
node_st *DGFTattribute_reference(node_st *node)
{
    CTI(CTI_WARN, true, "Not implemented DGFTattribute_reference");
    TRAVchildren(node);
    return node;
}

node_st *DGFTste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGFTsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGFTsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGFTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGFTienum(node_st *node)
{
    return node;
}

node_st *DGFTid(node_st *node)
{
    return node;
}

node_st *DGFTilifetime(node_st *node)
{
    return node;
}

node_st *DGFTlifetime_range(node_st *node)
{
    return node;
}

/**
 * @fn DGFTvisit_arg_list
 */
node_st *DGFTvisit_arg_list(node_st *node)
{
    CTI(CTI_WARN, true, "Not implemented DGFTvisit_arg_list");
    TRAVchildren(node);
    return node;
}

/**
 * @fn DGFTvisit
 */
node_st *DGFTvisit(node_st *node)
{
    CTI(CTI_WARN, true, "Not implemented DGFTvisit");
    TRAVchildren(node);
    return node;
}

/**
 * @fn DGFTvisit_sequence_eval
 */
node_st *DGFTvisit_sequence_eval(node_st *node)
{
    CTI(CTI_WARN, true, "Not implemented DGFTvisit_sequence_eval");
    TRAVchildren(node);
    return node;
}

/**
 * @fn DGFTvisit_sequence_dummy
 */
node_st *DGFTvisit_sequence_dummy(node_st *node)
{
    CTI(CTI_WARN, true, "Not implemented DGFTvisit_sequence_dummy");
    TRAVchildren(node);
    return node;
}

/**
 * @fn DGFTvisit_sequence_visit
 */
node_st *DGFTvisit_sequence_visit(node_st *node)
{
    CTI(CTI_WARN, true, "Not implemented DGFTvisit_sequence_visit");
    TRAVchildren(node);
    return node;
}
