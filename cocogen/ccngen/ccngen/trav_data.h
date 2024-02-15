#pragma once
#include "ccngen/ccn_defs.h"
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

struct data_sav {
    int errors;
    htable_stptr graph_nodes;
    node_st *symboltable;
};

struct data_cpr {
    htable_stptr found_attributes;
    htable_stptr found_children;
    node_st *curr_node;
    node_st *symboltable;
};

struct data_cha {
    node_st *curr_node;
    bool in_nodeset;
};

struct data_pra {
    node_st *curr_nodeset;
    node_st *symboltable;
};

struct data_nct {
    htable_stptr built_tables;
    htable_stptr added_refs;
    node_st *last_entry;
    node_st *curr_set;
    node_st *symboltable;
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

struct data_csm {
    node_st *curr_child;
    node_st *ste;
};

// Ignore empty struct warning
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-empty-struct"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

union TRAV_DATA {
    struct data_slc *setliteralcontains;
    struct data_sld *setliteraldifference;
    struct data_slis *setliteralintersect;
    struct data_slu *setliteralunion;
    struct data_sli *setliteralinsert;
    struct data_sav *scheduleattributevisits;
    struct data_cpr *checkproductrules;
    struct data_cha *checkattributes;
    struct data_pra *propagateattributes;
    struct data_nct *nodesetchildtable;
    struct data_dgcht *dynamicgenchecktraversal;
    struct data_dgt_f *dynamicgentravfunctions;
    struct data_dgtdt *dynamicgentravdatatables;
    struct data_csm *childrensetmandatory;
};

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#define DATA_SLC_GET() (TRAVgetCurrent()->trav_data.setliteralcontains)
#define DATA_SLD_GET() (TRAVgetCurrent()->trav_data.setliteraldifference)
#define DATA_SLIS_GET() (TRAVgetCurrent()->trav_data.setliteralintersect)
#define DATA_SLU_GET() (TRAVgetCurrent()->trav_data.setliteralunion)
#define DATA_SLI_GET() (TRAVgetCurrent()->trav_data.setliteralinsert)
#define DATA_SAV_GET() (TRAVgetCurrent()->trav_data.scheduleattributevisits)
#define DATA_CPR_GET() (TRAVgetCurrent()->trav_data.checkproductrules)
#define DATA_CHA_GET() (TRAVgetCurrent()->trav_data.checkattributes)
#define DATA_PRA_GET() (TRAVgetCurrent()->trav_data.propagateattributes)
#define DATA_NCT_GET() (TRAVgetCurrent()->trav_data.nodesetchildtable)
#define DATA_DGCHT_GET() (TRAVgetCurrent()->trav_data.dynamicgenchecktraversal)
#define DATA_DGT_F_GET() (TRAVgetCurrent()->trav_data.dynamicgentravfunctions)
#define DATA_DGTDT_GET() (TRAVgetCurrent()->trav_data.dynamicgentravdatatables)
#define DATA_CSM_GET() (TRAVgetCurrent()->trav_data.childrensetmandatory)
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
void TRAVdataInitscheduleAttributeVisits(struct ccn_trav *trav);
void TRAVdataFreescheduleAttributeVisits(struct ccn_trav *trav);
void TRAVdataInitcheckProductRules(struct ccn_trav *trav);
void TRAVdataFreecheckProductRules(struct ccn_trav *trav);
void TRAVdataInitcheckAttributes(struct ccn_trav *trav);
void TRAVdataFreecheckAttributes(struct ccn_trav *trav);
void TRAVdataInitpropagateAttributes(struct ccn_trav *trav);
void TRAVdataFreepropagateAttributes(struct ccn_trav *trav);
void TRAVdataInitnodesetChildTable(struct ccn_trav *trav);
void TRAVdataFreenodesetChildTable(struct ccn_trav *trav);
void TRAVdataInitdynamicGenCheckTraversal(struct ccn_trav *trav);
void TRAVdataFreedynamicGenCheckTraversal(struct ccn_trav *trav);
void TRAVdataInitdynamicGenTravFunctions(struct ccn_trav *trav);
void TRAVdataFreedynamicGenTravFunctions(struct ccn_trav *trav);
void TRAVdataInitdynamicGenTravDataTables(struct ccn_trav *trav);
void TRAVdataFreedynamicGenTravDataTables(struct ccn_trav *trav);
void TRAVdataInitchildrenSetMandatory(struct ccn_trav *trav);
void TRAVdataFreechildrenSetMandatory(struct ccn_trav *trav);
