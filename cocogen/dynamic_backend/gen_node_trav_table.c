#include <stddef.h>
#include <stdio.h>
#include <globals.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "frontend/reachability.h"

node_st *curr_trav_prefix = NULL;
int curr_trav_index = 0;
extern bool in_set(node_st *node, node_st *vale);
extern int **reachability_matrix;

node_st *DGNFTTast(node_st *node)
{
    return node;
}

node_st *DGNFTTinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    const int reachability = reachability_matrix[curr_trav_index][INODE_INDEX(node)];
    if (reachability == RCB_NODE_HANDLED_BY_USER) {
        OUT("&%s%s", ID_UPR(curr_trav_prefix), ID_LWR(INODE_NAME(node)));
    } else if (reachability == RCB_NODE_HANDLED_BY_TRAV) {
        OUT("&TRAVchildren");
    } else {
        OUT("&TRAVnop");
    }

    OUT(", ");

    TRAVchildren(node);
    return node;
}

