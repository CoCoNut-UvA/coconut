#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"
#include "ccngen/trav_data.h"

void DGTDTinit()
{
}

void DGTDTfini()
{

}

node_st *DGTDTast(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    struct data_dgtdt *data = DATA_DGTDT_GET();
    int indent = data->indent;

    OUT("const ccn_trav_data_ft trav_data_init_vtable[_TRAV_SIZE] = {TRAVdataNOP,");
    data->in_init_round = true;
    data->indent = indent;
    TRAVopt(AST_ITRAVERSALS(node));
    // For system traversal(check, free, copy)
    OUT("TRAVdataNOP, TRAVdataNOP, TRAVdataNOP");
    OUT("};\n");

    OUT("const ccn_trav_data_ft trav_data_free_vtable[_TRAV_SIZE] = {TRAVdataNOP,");
    data->in_init_round = false;
    data->indent = indent;
    TRAVopt(AST_ITRAVERSALS(node));
    // For system traversal(check, free, copy)
    OUT("TRAVdataNOP, TRAVdataNOP, TRAVdataNOP");
    OUT("};\n");

    return node;
}

node_st *DGTDTitraversal(node_st *node)
{
    GeneratorContext *ctx = globals.gen_ctx;
    struct data_dgtdt *data = DATA_DGTDT_GET();
    if (ITRAVERSAL_DATA(node)) {
        if (data->in_init_round) {
            OUT("TRAVdataInit%s,", ID_ORIG(ITRAVERSAL_NAME(node)));
        } else {
            OUT("TRAVdataFree%s,", ID_ORIG(ITRAVERSAL_NAME(node)));
        }
    } else {
        OUT("TRAVdataNOP,");
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}
