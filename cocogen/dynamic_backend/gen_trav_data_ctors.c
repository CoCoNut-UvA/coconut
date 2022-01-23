#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"

static bool has_user_type = false;
static bool check_user_type = false;
node_st *DGTDCitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    has_user_type = false;
    node_st *id = ITRAVERSAL_NAME(node);
    if (ITRAVERSAL_DATA(node)) {
        has_user_type = false;
        check_user_type = true;
        TRAVdo(ITRAVERSAL_DATA(node));
        check_user_type = false;
        OUT_FIELD("extern void %sinit()", ID_UPR(ITRAVERSAL_IPREFIX(node)));
        OUT_FIELD("extern void %sfini()", ID_UPR(ITRAVERSAL_IPREFIX(node)));
        
        OUT_START_FUNC("void TRAVdataInit%s(ccn_trav_st *trav)", ID_ORIG(id));
        OUT_FIELD("trav->trav_data.%s = MEMmalloc(sizeof(struct data_%s))",
                  ID_LWR(id), ID_LWR(ITRAVERSAL_IPREFIX(node)));
        OUT_FIELD("struct data_%s *data = trav->trav_data.%s", ID_LWR(ITRAVERSAL_IPREFIX(node)), ID_LWR(id));
        TRAVdo(ITRAVERSAL_DATA(node));
        OUT_FIELD("%sinit()", ID_UPR(ITRAVERSAL_IPREFIX(node)));
        OUT_END_FUNC();

        OUT_START_FUNC("void TRAVdataFree%s(ccn_trav_st *trav)", ID_ORIG(id));
        OUT_FIELD("%sfini()", ID_UPR(ITRAVERSAL_IPREFIX(node)));
        OUT_FIELD("MEMfree(trav->trav_data.%s)", ID_LWR(id));
        OUT_END_FUNC();
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}


node_st *DGTDCitravdata(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    if (check_user_type) {
        if (ITRAVDATA_TYPE(node) == AT_user) {
            has_user_type = true;
        } else {
            TRAVopt(ITRAVDATA_NEXT(node));
        }
        return node;
    }

    if (ITRAVDATA_TYPE(node) == AT_user) {
        // This might become a call to a function.
        OUT_FIELD("data->%s = 0", ID_ORIG(ITRAVDATA_NAME(node)));
    } else {
        OUT_FIELD("data->%s = %s", ID_ORIG(ITRAVDATA_NAME(node)), FMTattributeDefaultVal(ITRAVDATA_TYPE(node)));
    }

    TRAVopt(ITRAVDATA_NEXT(node));
    return node;
}
