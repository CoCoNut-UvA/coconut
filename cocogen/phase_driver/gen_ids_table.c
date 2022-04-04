#include <stdio.h>

#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "globals.h"

static node_st *ast;

static char *enum_action_pref = "CCNAC_ID";
static char *enum_action_name = "ccn_action_id";


node_st *GITast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    GNopenSourceFile(ctx, "action_handling.c");
    OUT("#include <stddef.h>\n");
    OUT("#include \"ccn/action_types.h\"\n");
    OUT("#include \"ccngen/action_handling.h\"\n");
    OUT("#include \"ccngen/enum.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"ccngen/gate.h\"\n");
    OUT("#include \"ccn/phase_driver.h\"\n");

    ast = node;
    OUT("enum %s cleanup_ids_table[2] = { %s_free, %s_NULL };\n", enum_action_name, enum_action_pref, enum_action_pref);
    TRAVchildren(node);
    return node;
}

node_st *GITiactions(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("%s_%s, ", enum_action_pref, ID_UPR(IACTIONS_REFERENCE(node)));
    TRAVopt(IACTIONS_NEXT(node));
    return node;
}

node_st *GITiphase(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT("enum %s %s_ids_table[] = {", enum_action_name, ID_LWR(IPHASE_NAME(node)));
    GNindentIncrease(ctx);
    TRAVopt(IPHASE_IACTIONS(node));
    OUT("%s_NULL, ", enum_action_pref);
    OUT_ENUM_END();
    TRAVopt(IPHASE_NEXT(node));
    return node;
}

