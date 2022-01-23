#include <stdio.h>
#include "gen_helpers/out_macros.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "globals.h"


static void do_enum(char *name, node_st *node, char *prefix)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_ENUM("%s", name);
    OUT_ENUM_FIELD("%sNULL", prefix);
    TRAVopt(node);
    if (STReq("TRAV_", prefix)) {
        OUT_ENUM_FIELD("TRAV_free");
        OUT_ENUM_FIELD("TRAV_check");
        OUT_ENUM_FIELD("TRAV_cpy");
    }
    OUT_ENUM_FIELD("_%sSIZE", prefix);
    OUT_ENUM_END();
}

node_st *DGEast(node_st *node)
{
    do_enum("nodesettype", AST_INODESETS(node), "NS_");
    do_enum("ccn_nodetype", AST_INODES((node)), "NT_");
    do_enum("ccn_traversal_type", AST_ITRAVERSALS(node), "TRAV_");
    do_enum("ccn_pass_type", AST_IPASSES(node), "PASS_");

    return node;
}


node_st *DGEitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_ENUM_FIELD("TRAV_%s", ID_UPR(ITRAVERSAL_IPREFIX(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGEipass(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_ENUM_FIELD("PASS_%s", ID_UPR(IPASS_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGEinode(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_ENUM_FIELD("NT_%s", ID_UPR(INODE_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGEinodeset(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_ENUM_FIELD("NS_%s", ID_UPR(INODESET_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGEchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}

