#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"

static int indent = 0;
node_st *DGTDCitraversal(node_st *node)
{
    FILE *fp = globals.fp;
    node_st *id = ITRAVERSAL_NAME(node);
    if (ITRAVERSAL_DATA(node)) {
        OUT_START_FUNC("void TRAVdataInit%s(ccn_trav_st *trav)", ID_ORIG(id));
        OUT_FIELD("trav->trav_data.%s = MEMmalloc(sizeof(struct trav_data_%s))",
                  ID_LWR(id), ID_LWR(id));
        OUT_FIELD("struct trav_data_%s *data = trav->trav_data.%s", ID_LWR(id), ID_LWR(id));
        TRAVdo(ITRAVERSAL_DATA(node));
        OUT_END_FUNC();

        OUT_START_FUNC("void TRAVdataFree%s(ccn_trav_st *trav)", ID_ORIG(id));
        OUT_FIELD("MEMfree(trav->trav_data.%s)", ID_LWR(id));
        OUT_END_FUNC();
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}


node_st *DGTDCitravdata(node_st *node)
{
    FILE *fp = globals.fp;

    OUT_FIELD("data->%s = %s", ID_ORIG(ITRAVDATA_NAME(node)), FMTattributeDefaultVal(ITRAVDATA_TYPE(node)));

    TRAVopt(ITRAVDATA_NEXT(node));
    return node;
}
