#include <stdlib.h>
#include "ccngen/trav_data.h"
#include "ccn/dynamic_core.h"
#include "palm/memory.h"
void TRAVdataInitsetliteralContains(ccn_trav_st *trav) {
    trav->trav_data.setliteralcontains = MEMmalloc(sizeof(struct data_slc));
    struct data_slc *data = trav->trav_data.setliteralcontains;
    data->contains = false;
    data->lookup = NULL;
}

void TRAVdataFreesetliteralContains(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteralcontains);
}

void TRAVdataInitsetliteralDifference(ccn_trav_st *trav) {
    trav->trav_data.setliteraldifference = MEMmalloc(sizeof(struct data_sld));
    struct data_sld *data = trav->trav_data.setliteraldifference;
    data->new = NULL;
    data->right = NULL;
}

void TRAVdataFreesetliteralDifference(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteraldifference);
}

void TRAVdataInitsetliteralIntersect(ccn_trav_st *trav) {
    trav->trav_data.setliteralintersect = MEMmalloc(sizeof(struct data_slis));
    struct data_slis *data = trav->trav_data.setliteralintersect;
    data->new = NULL;
    data->src = NULL;
}

void TRAVdataFreesetliteralIntersect(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteralintersect);
}

void TRAVdataInitsetliteralUnion(ccn_trav_st *trav) {
    trav->trav_data.setliteralunion = MEMmalloc(sizeof(struct data_slu));
    struct data_slu *data = trav->trav_data.setliteralunion;
    data->dst = NULL;
}

void TRAVdataFreesetliteralUnion(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteralunion);
}

void TRAVdataInitsetliteralInsert(ccn_trav_st *trav) {
    trav->trav_data.setliteralinsert = MEMmalloc(sizeof(struct data_sli));
    struct data_sli *data = trav->trav_data.setliteralinsert;
    data->inserted = false;
    data->value = NULL;
}

void TRAVdataFreesetliteralInsert(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteralinsert);
}

void TRAVdataInitdynamic_genCheckTraversal(ccn_trav_st *trav) {
    trav->trav_data.dynamic_genchecktraversal = MEMmalloc(sizeof(struct data_dgcht));
    struct data_dgcht *data = trav->trav_data.dynamic_genchecktraversal;
    data->lifetime_target = 0;
}

void TRAVdataFreedynamic_genCheckTraversal(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.dynamic_genchecktraversal);
}

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

