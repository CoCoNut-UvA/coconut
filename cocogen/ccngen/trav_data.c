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
void TRAVdataInitdynamic_genCheckTraversal(ccn_trav_st *trav) {
    trav->trav_data.dynamic_genchecktraversal = MEMmalloc(sizeof(struct data_dgcht));
    struct data_dgcht *data = trav->trav_data.dynamic_genchecktraversal;
    data->lifetime_target = 0;
    DGCHTinit();
}

void TRAVdataFreedynamic_genCheckTraversal(ccn_trav_st *trav) {
    DGCHTfini();
    MEMfree(trav->trav_data.dynamic_genchecktraversal);
}

extern void DGTDTinit();
extern void DGTDTfini();
void TRAVdataInitdynamic_genTravDataTables(ccn_trav_st *trav) {
    trav->trav_data.dynamic_gentravdatatables = MEMmalloc(sizeof(struct data_dgtdt));
    struct data_dgtdt *data = trav->trav_data.dynamic_gentravdatatables;
    data->indent = 0;
    data->in_init_round = 0;
    data->file = 0;
    DGTDTinit();
}

void TRAVdataFreedynamic_genTravDataTables(ccn_trav_st *trav) {
    DGTDTfini();
    MEMfree(trav->trav_data.dynamic_gentravdatatables);
}

