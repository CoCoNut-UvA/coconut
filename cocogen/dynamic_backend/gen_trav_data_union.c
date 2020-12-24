#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"

static int indent = 0;
node_st *DGTDUast(node_st *node)
{
    FILE *fp = globals.fp;
    OUT_UNION("TRAV_DATA");
    TRAVopt(AST_ITRAVERSALS(node));
    OUT_STRUCT_END();

    return node;
}


node_st *DGTDUitraversal(node_st *node)
{
    FILE *fp = globals.fp;
    node_st *id = ITRAVERSAL_NAME(node);
    if (ITRAVERSAL_DATA(node)) {
        OUT_FIELD("struct trav_data_%s *%s", ID_LWR(id), ID_LWR(id));
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}
