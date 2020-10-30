
#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"

node_st *DGTDSitraversal(node_st *node)
{
    TRAVchildren(node);
    return node;
}


node_st *DGTDSitravdata(node_st *node)
{
    FILE *fp = stdout;
    int indent = 0;
    if (ITRAVDATA_TYPE(node) == AT_link) {
        OUT("%s *%s\n", ID_ORIG(ITRAVDATA_TYPE_REFERENCE(node)), ID_ORIG(ITRAVDATA_NAME(node)));
    } else {
        OUT("%s %s\n", "X", ID_ORIG(ITRAVDATA_NAME(node)));
    }
    TRAVopt(ITRAVDATA_NEXT(node));
    return node;
}
