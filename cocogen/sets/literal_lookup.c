#include "ccngen/ast.h"
#include <string.h>
#include <assert.h>
#include "ccn/dynamic_core.h"
#include "ccngen/trav.h"

void SLCinit() { return; }
void SLCfini() { return; }


node_st *SLCsetliteral(node_st *node)
{
    struct data_slc *data = DATA_SLC_GET();
    assert(NODE_TYPE(data->lookup) == NT_ID);
    int cmp = strcmp(ID_LWR(data->lookup), ID_LWR(SETLITERAL_REFERENCE(node)));

    if (cmp == 0) {
        data->contains = true;
    } else if (cmp < 0) {
        TRAVleft(node);
    } else {
        TRAVright(node);
    }

    return node;
}


