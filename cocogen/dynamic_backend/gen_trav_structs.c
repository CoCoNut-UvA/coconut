#include "palm/dbug.h"
#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"
#include "frontend/symboltable.h"

static node_st *ast;
static node_st *ste;

node_st *DGTDSast(node_st *node)
{
    ast = node;
    ste = AST_STABLE(ast);
    TRAVchildren(ast);
    return node;
}

node_st *DGTDSitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (ITRAVERSAL_DATA(node)) {
        OUT_STRUCT("data_%s", ID_LWR(ITRAVERSAL_IPREFIX(node)));
        TRAVdo(ITRAVERSAL_DATA(node));
        OUT_STRUCT_END();
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}


node_st *DGTDSitravdata(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (ITRAVDATA_TYPE(node) == AT_link) {
        OUT_FIELD("node_st *%s", ID_ORIG(ITRAVDATA_NAME(node)));
    } else if (ITRAVDATA_TYPE(node) == AT_link_or_enum) {
        node_st *corr_enum = STlookup(ste, ITRAVDATA_TYPE_REFERENCE(node));
        DBUG_ASSERT(corr_enum != NULL, "Oeps, invalid system.");
        DBUG_ASSERT(NODE_TYPE(corr_enum) == NT_IENUM, "Travdata type invalid.");
        OUT_FIELD("enum %s %s", ID_ORIG(IENUM_NAME(corr_enum)), ID_ORIG(ITRAVDATA_NAME(node)));
    }
    else if (ITRAVDATA_TYPE(node) == AT_user) {
        AST_USES_UNSAFE(ast) = true;
        OUT_FIELD("%s %s", ID_ORIG(ITRAVDATA_TYPE_REFERENCE(node)), ID_ORIG(ITRAVDATA_NAME(node)));
    } else {
        OUT_FIELD("%s %s", FMTattributeTypeToString(ITRAVDATA_TYPE(node)), ID_ORIG(ITRAVDATA_NAME(node)));
    }
    TRAVopt(ITRAVDATA_NEXT(node));
    return node;
}

