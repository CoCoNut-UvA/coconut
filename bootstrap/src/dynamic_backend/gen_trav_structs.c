
#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"

node_st *DGTDSitraversal(node_st *node)
{
    int indent = 0;
    FILE *fp = globals.fp;
    if (ITRAVERSAL_DATA(node)) {
        OUT_STRUCT("trav_data_%s", ID_LWR(ITRAVERSAL_NAME(node)));
        TRAVdo(ITRAVERSAL_DATA(node));
        OUT_STRUCT_END();
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}


node_st *DGTDSitravdata(node_st *node)
{
    FILE *fp = globals.fp;
    int indent = 0;
    if (ITRAVDATA_TYPE(node) == AT_link) {
        OUT_FIELD("node_st *%s", ID_ORIG(ITRAVDATA_NAME(node)));
    } else {
        OUT_FIELD("%s %s", FMTattributeTypeToString(ITRAVDATA_TYPE(node)), ID_ORIG(ITRAVDATA_NAME(node)));
    }
    TRAVopt(ITRAVDATA_NEXT(node));
    return node;
}
