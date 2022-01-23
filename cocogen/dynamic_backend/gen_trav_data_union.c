#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"

node_st *DGTDUast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    OUT_UNION("TRAV_DATA");
    TRAVopt(AST_ITRAVERSALS(node));
    OUT_STRUCT_END();

    return node;
}


node_st *DGTDUitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    node_st *id = ITRAVERSAL_NAME(node);
    if (ITRAVERSAL_DATA(node)) {
        OUT_FIELD("struct data_%s *%s", ID_LWR(ITRAVERSAL_IPREFIX(node)), ID_LWR(id));
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}
