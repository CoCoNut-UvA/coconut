#pragma once
#include "ccn/ccn_types.h"
#include "unsafe_types.h"
struct trav_data_dynamic_genchecktraversal {
int lifetime_target;
};

struct trav_data_dynamic_gentravdatatables {
int indent;
int in_init_round;
fileptr file;
};

union TRAV_DATA {
    struct trav_data_dynamic_genchecktraversal *dynamic_genchecktraversal;
    struct trav_data_dynamic_gentravdatatables *dynamic_gentravdatatables;
};

#define DYNAMIC_GENCHECKTRAVERSAL_DATA (TRAVgetCurrent()->trav_data.dynamic_genchecktraversal)
#define DYNAMIC_GENTRAVDATATABLES_DATA (TRAVgetCurrent()->trav_data.dynamic_gentravdatatables)
void TRAVdataInitdynamic_genCheckTraversal(struct ccn_trav *trav);
void TRAVdataFreedynamic_genCheckTraversal(struct ccn_trav *trav);
void TRAVdataInitdynamic_genTravDataTables(struct ccn_trav *trav);
void TRAVdataFreedynamic_genTravDataTables(struct ccn_trav *trav);
