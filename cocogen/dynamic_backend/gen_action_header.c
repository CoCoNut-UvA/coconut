#include <stddef.h>
#include <stdio.h>

#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "globals.h"
#include "dynamic_backend/gen_helpers.h"
#include "../gen_helpers.h"

char *current_trav_prefix = NULL;
static node_st *ast;

node_st *DGAHast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "actions.h");
    OUT_STATEMENT(GH_ENSURE_DEFAULT_NODE_TYPE());
    ast = node;
    TRAVchildren(node);
    return node;
}

node_st *DGAHitraversal(node_st *node)
{
    current_trav_prefix = DGH_TRAVERSAL_PREFIX(node);
    if (ITRAVERSAL_INODES(node)) {
        TRAVdo(ITRAVERSAL_INODES(node));
    } else {
        TRAVstart(AST_INODES(ast), TRAV_DGTA);
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    current_trav_prefix = NULL;
    return node;
}

node_st *DGAHipass(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *prefix = "";
    if (IPASS_IPREFIX(node)) {
        prefix = DGH_PASS_PREFIX(node);
    }
    OUT_FIELD(DGH_PASS_FUNC_SIG(), prefix, DGH_PASS_NAME(node), "");
    TRAVopt(IPASS_NEXT(node));
    return node;
}

node_st *DGAHinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_FIELD(DGH_TRAV_FUNC_SIG(), "CPY", DGH_TRAVERSAL_TARGET_ID(INODE_NAME(node)), "");
    OUT_FIELD(DGH_TRAV_FUNC_SIG(), "DEL", DGH_TRAVERSAL_TARGET_ID(INODE_NAME(node)), "");
    OUT_FIELD(DGH_TRAV_FUNC_SIG(), "CHK", DGH_TRAVERSAL_TARGET_ID(INODE_NAME(node)), "");

    TRAVopt(INODE_NEXT(node));

    return node;
}

node_st *DGAHsetliteral(node_st *node)
{
    TRAVopt(SETLITERAL_REFERENCE(node));
    TRAVchildren(node);
    return node;
}


node_st *DGAHid(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (!current_trav_prefix) {
        return node;
    }
    OUT_FIELD(DGH_TRAV_FUNC_SIG(), current_trav_prefix, DGH_TRAVERSAL_TARGET_ID(node), "");
    TRAVchildren(node);
    return node;
}
