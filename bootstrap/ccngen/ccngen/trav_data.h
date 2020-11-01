#pragma once
#include "ccn/ccn_types.h"
struct trav_data_dynamic_gentravdatatables {
int indent;
int in_init_round;
};

union TRAV_DATA {
    struct trav_data_dynamic_gentravdatatables *dynamic_gentravdatatables;
};

#define DYNAMIC_GENTRAVDATATABLES_DATA (TRAVgetCurrent()->trav_data.dynamic_gentravdatatables)
void TRAVdataInitdynamic_genTravDataTables(struct ccn_trav *trav);
void TRAVdataFreedynamic_genTravDataTables(struct ccn_trav *trav);
