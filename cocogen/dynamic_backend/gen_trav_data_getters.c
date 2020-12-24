#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"

node_st *DGTDGitraversal(node_st *node)
{
    if (ITRAVERSAL_DATA(node)) {
        FILE *fp = globals.fp;
        int indent = 0;
        node_st *id =  ITRAVERSAL_NAME(node);

        OUT("#define %s_DATA (TRAVgetCurrent()->trav_data.%s)\n",
            ID_UPR(id), ID_LWR(id));
    }

    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}
