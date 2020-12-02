#include <stdlib.h>
#include "ccngen/trav_data.h"
#include "ccn/dynamic_core.h"
#include "palm/memory.h"
void TRAVdataInitsetliteralContains(ccn_trav_st *trav) {
    trav->trav_data.setliteralcontains = MEMmalloc(sizeof(struct trav_data_setliteralcontains));
    struct trav_data_setliteralcontains *data = trav->trav_data.setliteralcontains;
    data->contains = false;
    data->lookup = NULL;
}

void TRAVdataFreesetliteralContains(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteralcontains);
}

void TRAVdataInitsetliteralIntersect(ccn_trav_st *trav) {
    trav->trav_data.setliteralintersect = MEMmalloc(sizeof(struct trav_data_setliteralintersect));
    struct trav_data_setliteralintersect *data = trav->trav_data.setliteralintersect;
    data->new = NULL;
    data->src = NULL;
}

void TRAVdataFreesetliteralIntersect(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteralintersect);
}

void TRAVdataInitsetliteralUnion(ccn_trav_st *trav) {
    trav->trav_data.setliteralunion = MEMmalloc(sizeof(struct trav_data_setliteralunion));
    struct trav_data_setliteralunion *data = trav->trav_data.setliteralunion;
    data->dst = NULL;
}

void TRAVdataFreesetliteralUnion(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteralunion);
}

void TRAVdataInitsetliteralInsert(ccn_trav_st *trav) {
    trav->trav_data.setliteralinsert = MEMmalloc(sizeof(struct trav_data_setliteralinsert));
    struct trav_data_setliteralinsert *data = trav->trav_data.setliteralinsert;
    data->inserted = false;
    data->value = NULL;
}

void TRAVdataFreesetliteralInsert(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.setliteralinsert);
}

void TRAVdataInitdynamic_genCheckTraversal(ccn_trav_st *trav) {
    trav->trav_data.dynamic_genchecktraversal = MEMmalloc(sizeof(struct trav_data_dynamic_genchecktraversal));
    struct trav_data_dynamic_genchecktraversal *data = trav->trav_data.dynamic_genchecktraversal;
    data->lifetime_target = 0;
}

void TRAVdataFreedynamic_genCheckTraversal(ccn_trav_st *trav) {
    MEMfree(trav->trav_data.dynamic_genchecktraversal);
}

void TRAVdataInitdynamic_genTravDataTables(ccn_trav_st *trav) {
    trav->trav_data.dynamic_gentravdatatables = MEMmalloc(sizeof(struct trav_data_dynamic_gentravdatatables));
    struct trav_data_dynamic_gentravdatatables *data = trav->trav_data.dynamic_gentravdatatables;
    data->indent = 0;
    data->in_init_round = 0;
    data->file = 0;
    DGTDTinit();
}

void TRAVdataFreedynamic_genTravDataTables(ccn_trav_st *trav) {
    DGTDTfini();
    MEMfree(trav->trav_data.dynamic_gentravdatatables);
}

