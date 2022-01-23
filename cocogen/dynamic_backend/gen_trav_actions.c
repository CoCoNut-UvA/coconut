#include <stddef.h>
#include <stdio.h>
#include <globals.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

extern char *current_trav_prefix;

node_st *DGTAast(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGTAinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_FIELD("struct ccn_node *%s%s(struct ccn_node *node)", current_trav_prefix, ID_LWR(INODE_NAME(node)));
    TRAVopt(INODE_NEXT(node));
    return node;
}
