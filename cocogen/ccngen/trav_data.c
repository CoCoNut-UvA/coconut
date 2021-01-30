#include <stdlib.h>
#include "ccngen/trav_data.h"
#include "ccn/dynamic_core.h"
#include "palm/memory.h"
extern void SLCinit();
extern void SLCfini();
void TRAVdataInitsetliteralContains(ccn_trav_st *trav) {
    trav->trav_data.setliteralcontains = MEMmalloc(sizeof(struct data_slc));
    struct data_slc *data = trav->trav_data.setliteralcontains;
    data->contains = false;
    data->lookup = NULL;
    SLCinit();
}

void TRAVdataFreesetliteralContains(ccn_trav_st *trav) {
    SLCfini();
    MEMfree(trav->trav_data.setliteralcontains);
}

extern void SLDinit();
extern void SLDfini();
void TRAVdataInitsetliteralDifference(ccn_trav_st *trav) {
    trav->trav_data.setliteraldifference = MEMmalloc(sizeof(struct data_sld));
    struct data_sld *data = trav->trav_data.setliteraldifference;
    data->new = NULL;
    data->right = NULL;
    SLDinit();
}

void TRAVdataFreesetliteralDifference(ccn_trav_st *trav) {
    SLDfini();
    MEMfree(trav->trav_data.setliteraldifference);
}

extern void SLISinit();
extern void SLISfini();
void TRAVdataInitsetliteralIntersect(ccn_trav_st *trav) {
    trav->trav_data.setliteralintersect = MEMmalloc(sizeof(struct data_slis));
    struct data_slis *data = trav->trav_data.setliteralintersect;
    data->new = NULL;
    data->src = NULL;
    SLISinit();
}

void TRAVdataFreesetliteralIntersect(ccn_trav_st *trav) {
    SLISfini();
    MEMfree(trav->trav_data.setliteralintersect);
}

extern void SLUinit();
extern void SLUfini();
void TRAVdataInitsetliteralUnion(ccn_trav_st *trav) {
    trav->trav_data.setliteralunion = MEMmalloc(sizeof(struct data_slu));
    struct data_slu *data = trav->trav_data.setliteralunion;
    data->dst = NULL;
    SLUinit();
}

void TRAVdataFreesetliteralUnion(ccn_trav_st *trav) {
    SLUfini();
    MEMfree(trav->trav_data.setliteralunion);
}

extern void SLIinit();
extern void SLIfini();
void TRAVdataInitsetliteralInsert(ccn_trav_st *trav) {
    trav->trav_data.setliteralinsert = MEMmalloc(sizeof(struct data_sli));
    struct data_sli *data = trav->trav_data.setliteralinsert;
    data->inserted = false;
    data->value = NULL;
    SLIinit();
}

void TRAVdataFreesetliteralInsert(ccn_trav_st *trav) {
    SLIfini();
    MEMfree(trav->trav_data.setliteralinsert);
}

extern void DGCHTinit();
extern void DGCHTfini();
void TRAVdataInitdynamicGenCheckTraversal(ccn_trav_st *trav) {
    trav->trav_data.dynamicgenchecktraversal = MEMmalloc(sizeof(struct data_dgcht));
    struct data_dgcht *data = trav->trav_data.dynamicgenchecktraversal;
    data->lifetime_target = 0;
    DGCHTinit();
}

void TRAVdataFreedynamicGenCheckTraversal(ccn_trav_st *trav) {
    DGCHTfini();
    MEMfree(trav->trav_data.dynamicgenchecktraversal);
}

extern void DGT_Finit();
extern void DGT_Ffini();
void TRAVdataInitdynamicGenTravFunctions(ccn_trav_st *trav) {
    trav->trav_data.dynamicgentravfunctions = MEMmalloc(sizeof(struct data_dgt_f));
    struct data_dgt_f *data = trav->trav_data.dynamicgentravfunctions;
    data->curr_node = NULL;
    DGT_Finit();
}

void TRAVdataFreedynamicGenTravFunctions(ccn_trav_st *trav) {
    DGT_Ffini();
    MEMfree(trav->trav_data.dynamicgentravfunctions);
}

extern void DGTDTinit();
extern void DGTDTfini();
void TRAVdataInitdynamicGenTravDataTables(ccn_trav_st *trav) {
    trav->trav_data.dynamicgentravdatatables = MEMmalloc(sizeof(struct data_dgtdt));
    struct data_dgtdt *data = trav->trav_data.dynamicgentravdatatables;
    data->indent = 0;
    data->in_init_round = 0;
    data->file = 0;
    DGTDTinit();
}

void TRAVdataFreedynamicGenTravDataTables(ccn_trav_st *trav) {
    DGTDTfini();
    MEMfree(trav->trav_data.dynamicgentravdatatables);
}

