#include <stdlib.h>
#include "ccngen/trav_data.h"
#include "ccn/dynamic_core.h"
#include "palm/memory.h"
void TRAVdataInitdynamic_genTravDataTables(ccn_trav_st *trav) {
    trav->trav_data.dynamic_gentravdatatables = MEMmalloc(sizeof(struct trav_data_dynamic_gentravdatatables));
    struct trav_data_dynamic_gentravdatatables *data = trav->trav_data.dynamic_gentravdatatables;
    data->indent = 0;
    data->in_init_round = 0;
}

void TRAVdataFreedynamic_genTravDataTables(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.dynamic_gentravdatatables);
}

