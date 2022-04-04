
#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "globals.h"
#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"

static node_st *ast;

static char *enum_action_pref = "CCNAC_ID";
static char *enum_action_name = "ccn_action_id";


node_st *GAAast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    ast = node;

    OUT("static struct ccn_action ccn_action_array[] = {\n");

    TRAVchildren(node);

    OUT("{CCN_ACTION_TRAVERSAL, %s_free, \"Free\", .traversal = "
        "{TRAV_free,},},\n", enum_action_pref);

    OUT("{CCN_ACTION_PHASE, %s_cleanup, \"Cleanup\", .phase = {NULL, NT_%s, "
        "cleanup_ids_table, false, %s_cleanup,},},\n",
        enum_action_pref, ID_UPR(INODE_NAME(AST_ROOT_NODE(ast))), enum_action_pref);

    OUT("};\n\n");

    OUT_START_FUNC(
            "struct ccn_action *CCNgetActionFromID(enum %s action_id)", enum_action_name);
    OUT_STATEMENT("return &ccn_action_array[action_id]");
    OUT_END_FUNC();

    return node;
}

node_st *GAAiphase(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *name_upr = ID_UPR(IPHASE_NAME(node));
    OUT("{CCN_ACTION_PHASE, %s_%s, \"%s\", .phase = {%s, NT_%s, "
        "%s_ids_table, %s, %s_%s,},},\n", enum_action_pref, name_upr, ID_ORIG(IPHASE_NAME(node)), IPHASE_GATE_FUNC(node) ? ID_ORIG(IPHASE_GATE_FUNC(node)) : "NULL",
        ID_UPR(INODE_NAME(AST_ROOT_NODE(ast))), ID_LWR(IPHASE_NAME(node)), IPHASE_IS_CYCLE(node) ? "true": "false" ,enum_action_pref, name_upr);

    TRAVopt(IPHASE_NEXT(node));
    return node;
}


node_st *GAAitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *name_upr = ID_UPR(ITRAVERSAL_NAME(node));
    OUT("{CCN_ACTION_TRAVERSAL, %s_%s, \"%s\", .traversal = "
        "{TRAV_%s,},},\n", enum_action_pref, name_upr, ID_ORIG(ITRAVERSAL_NAME(node)), ID_UPR(ITRAVERSAL_IPREFIX(node)));

    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}

node_st *GAAipass(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    char *name_upr = ID_UPR(IPASS_NAME(node));
    OUT("{CCN_ACTION_PASS, %s_%s, \"%s\", .pass = {PASS_%s,},},\n",
        enum_action_pref, name_upr, ID_ORIG(IPASS_NAME(node)), name_upr);

    TRAVopt(IPASS_NEXT(node));
    return node;
}
