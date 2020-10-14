#include <stddef.h>
#include <stdio.h>
#include <globals.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

void *DGNFTTallocTravData()
{
    return 0;
}

void DGNFTTfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int child_num = 0;
static char *node_type_enum_prefix = "NT_";

node_st *curr_trav_prefix = NULL;
node_st *curr_trav_nodes = NULL;
extern bool in_set(node_st *node, node_st *vale);

node_st *DGNFTTast(node_st *node)
{
    fp = globals.fp;
    return node;
}

node_st *DGNFTTiactions(node_st *node)
{
    return node;
}

node_st *DGNFTTiphase(node_st *node)
{
    return node;
}

node_st *DGNFTTitraversal(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNFTTipass(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGNFTTinode(node_st *node)
{
    fp = globals.fp;
    if (!curr_trav_nodes || in_set(curr_trav_nodes, INODE_NAME(node))) {
        OUT("&%s%s", ID_UPR(curr_trav_prefix), ID_LWR(INODE_NAME(node)));
    } else {
        OUT("&TRAVchildren");
    }
    OUT(", ");

    TRAVchildren(node);
    return node;
}

node_st *DGNFTTinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGNFTTchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGNFTTattribute(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *DGNFTTste(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNFTTsetoperation(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNFTTsetliteral(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *DGNFTTsetreference(node_st *node)
{

    TRAVchildren(node);
    return node;
}

static char *enum_prefx;
node_st *DGNFTTienum(node_st *node)
{
    return node;
}

node_st *DGNFTTid(node_st *node)
{
    return node;
}
