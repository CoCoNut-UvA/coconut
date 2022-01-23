#include <stdio.h>
#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "globals.h"


node_st *DGEVast(node_st *node)
{
    TRAVopt(AST_ENUMS(node));
    return node;
}

static char *enum_prefx;
node_st *DGEVienum(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_ENUM("%s", ID_ORIG(IENUM_NAME(node)));
    enum_prefx = ID_UPR(IENUM_IPREFIX(node));
    OUT_ENUM_FIELD("%s_NULL", enum_prefx);
    TRAVopt(IENUM_VALS(node));
    OUT_ENUM_END();
    TRAVopt(IENUM_NEXT(node));
    return node;
}

node_st *DGEVid(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_ENUM_FIELD("%s_%s", enum_prefx, ID_LWR(node));
    TRAVchildren(node);
    return node;
}
