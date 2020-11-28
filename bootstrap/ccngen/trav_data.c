#include <stdlib.h>
#include "ccngen/trav_data.h"
#include "ccn/dynamic_core.h"
#include "palm/memory.h"
void TRAVdataInitdynamic_genCheckTraversal(ccn_trav_st *trav) {
    trav->trav_data.dynamic_genchecktraversal = MEMmalloc(sizeof(struct trav_data_dynamic_genchecktraversal));
    struct trav_data_dynamic_genchecktraversal *data = trav->trav_data.dynamic_genchecktraversal;
    data->lifetime_target = 0;
}

void TRAVdataFreedynamic_genCheckTraversal(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.dynamic_genchecktraversal);
}

void TRAVdataInitdynamic_genTravDataTables(ccn_trav_st *trav) {
    trav->trav_data.dynamic_gentravdatatables = MEMmalloc(sizeof(struct trav_data_dynamic_gentravdatatables));
    struct trav_data_dynamic_gentravdatatables *data = trav->trav_data.dynamic_gentravdatatables;
    data->indent = 0;
    data->in_init_round = 0;
    data->file = 0;
    DGTDTinit();
}

void TRAVdataFreedynamic_genTravDataTables(ccn_trav_st *trav) {
    DGTDTfini();
    MEMfree(trav->trav_data.dynamic_gentravdatatables);
}

