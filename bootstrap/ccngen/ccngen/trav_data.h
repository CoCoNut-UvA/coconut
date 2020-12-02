#pragma once
#include "ccn/ccn_types.h"
#include "unsafe_types.h"
#include "ccngen/ast.h"
struct trav_data_setliteralcontains {
bool contains;
node_st *lookup;
};

struct trav_data_setliteralintersect {
node_st *new;
node_st *src;
};

struct trav_data_setliteralunion {
node_st *dst;
};

struct trav_data_setliteralinsert {
bool inserted;
node_st *value;
};

struct trav_data_dynamic_genchecktraversal {
int lifetime_target;
};

struct trav_data_dynamic_gentravdatatables {
int indent;
int in_init_round;
fileptr file;
};

union TRAV_DATA {
    struct trav_data_setliteralcontains *setliteralcontains;
    struct trav_data_setliteralintersect *setliteralintersect;
    struct trav_data_setliteralunion *setliteralunion;
    struct trav_data_setliteralinsert *setliteralinsert;
    struct trav_data_dynamic_genchecktraversal *dynamic_genchecktraversal;
    struct trav_data_dynamic_gentravdatatables *dynamic_gentravdatatables;
};

#define SETLITERALCONTAINS_DATA (TRAVgetCurrent()->trav_data.setliteralcontains)
#define SETLITERALINTERSECT_DATA (TRAVgetCurrent()->trav_data.setliteralintersect)
#define SETLITERALUNION_DATA (TRAVgetCurrent()->trav_data.setliteralunion)
#define SETLITERALINSERT_DATA (TRAVgetCurrent()->trav_data.setliteralinsert)
#define DYNAMIC_GENCHECKTRAVERSAL_DATA (TRAVgetCurrent()->trav_data.dynamic_genchecktraversal)
#define DYNAMIC_GENTRAVDATATABLES_DATA (TRAVgetCurrent()->trav_data.dynamic_gentravdatatables)
void TRAVdataInitsetliteralContains(struct ccn_trav *trav);
void TRAVdataFreesetliteralContains(struct ccn_trav *trav);
void TRAVdataInitsetliteralIntersect(struct ccn_trav *trav);
void TRAVdataFreesetliteralIntersect(struct ccn_trav *trav);
void TRAVdataInitsetliteralUnion(struct ccn_trav *trav);
void TRAVdataFreesetliteralUnion(struct ccn_trav *trav);
void TRAVdataInitsetliteralInsert(struct ccn_trav *trav);
void TRAVdataFreesetliteralInsert(struct ccn_trav *trav);
void TRAVdataInitdynamic_genCheckTraversal(struct ccn_trav *trav);
void TRAVdataFreedynamic_genCheckTraversal(struct ccn_trav *trav);
void TRAVdataInitdynamic_genTravDataTables(struct ccn_trav *trav);
void TRAVdataFreedynamic_genTravDataTables(struct ccn_trav *trav);
