#pragma once
#include "ccn/ccn_types.h"
#include "unsafe_types.h"
#include "ccngen/ast.h"
struct data_slc {
bool contains;
node_st *lookup;
};

struct data_sld {
node_st *new;
node_st *right;
};

struct data_slis {
node_st *new;
node_st *src;
};

struct data_slu {
node_st *dst;
};

struct data_sli {
bool inserted;
node_st *value;
};

struct data_dgcht {
int lifetime_target;
};

struct data_dgtdt {
int indent;
int in_init_round;
fileptr file;
};

union TRAV_DATA {
    struct data_slc *setliteralcontains;
    struct data_sld *setliteraldifference;
    struct data_slis *setliteralintersect;
    struct data_slu *setliteralunion;
    struct data_sli *setliteralinsert;
    struct data_dgcht *dynamic_genchecktraversal;
    struct data_dgtdt *dynamic_gentravdatatables;
};

#define DATA_SLC_GET() (TRAVgetCurrent()->trav_data.setliteralcontains)
#define DATA_SLD_GET() (TRAVgetCurrent()->trav_data.setliteraldifference)
#define DATA_SLIS_GET() (TRAVgetCurrent()->trav_data.setliteralintersect)
#define DATA_SLU_GET() (TRAVgetCurrent()->trav_data.setliteralunion)
#define DATA_SLI_GET() (TRAVgetCurrent()->trav_data.setliteralinsert)
#define DATA_DGCHT_GET() (TRAVgetCurrent()->trav_data.dynamic_genchecktraversal)
#define DATA_DGTDT_GET() (TRAVgetCurrent()->trav_data.dynamic_gentravdatatables)
void TRAVdataInitsetliteralContains(struct ccn_trav *trav);
void TRAVdataFreesetliteralContains(struct ccn_trav *trav);
void TRAVdataInitsetliteralDifference(struct ccn_trav *trav);
void TRAVdataFreesetliteralDifference(struct ccn_trav *trav);
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
