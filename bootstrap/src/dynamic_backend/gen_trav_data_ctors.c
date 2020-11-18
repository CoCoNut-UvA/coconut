#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"

static int indent = 0;
static bool has_user_type = false;
node_st *DGTDCitraversal(node_st *node)
{
    has_user_type = false;
    FILE *fp = globals.fp;
    node_st *id = ITRAVERSAL_NAME(node);
    if (ITRAVERSAL_DATA(node)) {
        OUT_START_FUNC("void TRAVdataInit%s(ccn_trav_st *trav)", ID_ORIG(id));
        OUT_FIELD("trav->trav_data.%s = MEMmalloc(sizeof(struct trav_data_%s))",
                  ID_LWR(id), ID_LWR(id));
        OUT_FIELD("struct trav_data_%s *data = trav->trav_data.%s", ID_LWR(id), ID_LWR(id));
        TRAVdo(ITRAVERSAL_DATA(node));
        if (has_user_type) {
            OUT_FIELD("%sinit()", ID_UPR(ITRAVERSAL_IPREFIX(node)));
        }
        OUT_END_FUNC();

        OUT_START_FUNC("void TRAVdataFree%s(ccn_trav_st *trav)", ID_ORIG(id));
        if (has_user_type) {
            OUT_FIELD("%sfini()", ID_UPR(ITRAVERSAL_IPREFIX(node)));
        }
        OUT_FIELD("MEMfree(trav->trav_data.%s)", ID_LWR(id));
        OUT_END_FUNC();
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}


node_st *DGTDCitravdata(node_st *node)
{
    FILE *fp = globals.fp;
    if (ITRAVDATA_TYPE(node) == AT_user) {
        // This might become a call to a function.
        OUT_FIELD("data->%s = 0", ID_ORIG(ITRAVDATA_NAME(node)));
        has_user_type = true;
    } else {
        OUT_FIELD("data->%s = %s", ID_ORIG(ITRAVDATA_NAME(node)), FMTattributeDefaultVal(ITRAVDATA_TYPE(node)));
    }

    TRAVopt(ITRAVDATA_NEXT(node));
    return node;
}
