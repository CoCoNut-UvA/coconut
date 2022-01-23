#pragma once
#include"ccngen/ccn_defs.h"
#include "ccn/ccn_types.h"
#ifdef CCN_USES_UNSAFE
#include "user_types.h"
#endif
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

struct data_dgt_f {
    node_st *curr_node;
};

struct data_dgtdt {
    uint64_t indent;
    int in_init_round;
    fileptr file;
};

union TRAV_DATA {
    struct data_slc *setliteralcontains;
    struct data_sld *setliteraldifference;
    struct data_slis *setliteralintersect;
    struct data_slu *setliteralunion;
    struct data_sli *setliteralinsert;
    struct data_dgcht *dynamicgenchecktraversal;
    struct data_dgt_f *dynamicgentravfunctions;
    struct data_dgtdt *dynamicgentravdatatables;
};

#define DATA_SLC_GET() (TRAVgetCurrent()->trav_data.setliteralcontains)
#define DATA_SLD_GET() (TRAVgetCurrent()->trav_data.setliteraldifference)
#define DATA_SLIS_GET() (TRAVgetCurrent()->trav_data.setliteralintersect)
#define DATA_SLU_GET() (TRAVgetCurrent()->trav_data.setliteralunion)
#define DATA_SLI_GET() (TRAVgetCurrent()->trav_data.setliteralinsert)
#define DATA_DGCHT_GET() (TRAVgetCurrent()->trav_data.dynamicgenchecktraversal)
#define DATA_DGT_F_GET() (TRAVgetCurrent()->trav_data.dynamicgentravfunctions)
#define DATA_DGTDT_GET() (TRAVgetCurrent()->trav_data.dynamicgentravdatatables)
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
void TRAVdataInitdynamicGenCheckTraversal(struct ccn_trav *trav);
void TRAVdataFreedynamicGenCheckTraversal(struct ccn_trav *trav);
void TRAVdataInitdynamicGenTravFunctions(struct ccn_trav *trav);
void TRAVdataFreedynamicGenTravFunctions(struct ccn_trav *trav);
void TRAVdataInitdynamicGenTravDataTables(struct ccn_trav *trav);
void TRAVdataFreedynamicGenTravDataTables(struct ccn_trav *trav);
