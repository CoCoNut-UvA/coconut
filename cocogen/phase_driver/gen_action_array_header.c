
#include "assert.h"
#include <globals.h>
#include <stddef.h>
#include <stdio.h>

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"


static node_st *ast;
static size_t size = 0;

static char *enum_action_pref = "CCNAC_ID";
static char *enum_action_name = "ccn_action_id";


node_st *GAAHast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenIncludeFile(ctx, "action_handling.h");
    ast = node;

    OUT_ENUM("%s", enum_action_name);

    TRAVchildren(node);

    OUT_ENUM_FIELD("%s_%s", enum_action_pref, "check");
    OUT_ENUM_FIELD("%s_%s", enum_action_pref, "free");
    OUT_ENUM_FIELD("%s_%s", enum_action_pref, "cleanup");
    size += 3;
    OUT_ENUM_FIELD("%s_NULL", enum_action_pref);
    OUT_ENUM_END();

    OUT("#ifndef CCN_NUM_OF_ACTIONS\n");
    OUT("#define CCN_NUM_OF_ACTIONS %lu\n", size);
    OUT("#endif\n");

    OUT("#ifndef CCN_ROOT_TYPE\n");
    OUT("#define CCN_ROOT_TYPE NT_%s\n", ID_UPR(INODE_NAME(AST_ROOT_NODE(node))));
    OUT("#endif\n");

    OUT("#ifndef CCN_ROOT_ACTION\n");
    OUT("#define CCN_ROOT_ACTION %s_%s\n", enum_action_pref, ID_UPR(IPHASE_NAME(AST_START_PHASE(node))));
    OUT("#endif\n\n");

    return node;
}

node_st *GAAHiphase(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    size++;
    OUT_ENUM_FIELD("%s_%s", enum_action_pref, ID_UPR(IPHASE_NAME(node)));
    TRAVopt(IPHASE_NEXT(node));
    return node;
}

node_st *GAAHitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    size++;
    OUT_ENUM_FIELD("%s_%s", enum_action_pref, ID_UPR(ITRAVERSAL_NAME(node)));
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}

node_st *GAAHipass(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    size++;
    OUT_ENUM_FIELD("%s_%s", enum_action_pref, ID_UPR(IPASS_NAME(node)));
    TRAVopt(IPASS_NEXT(node));
    return node;
}
