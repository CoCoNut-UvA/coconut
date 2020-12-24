#include "ccngen/ast.h"
#include <string.h>
#include <assert.h>
#include "ccn/dynamic_core.h"


node_st *SLCsetliteral(node_st *node)
{
    struct trav_data_setliteralcontains *data = SETLITERALCONTAINS_DATA;
    assert(NODE_TYPE(data->lookup) == NT_ID);
    int cmp = strcmp(ID_LWR(data->lookup), ID_LWR(SETLITERAL_REFERENCE(node)));

    if (cmp == 0) {
        data->contains = true;
    } else if (cmp < 0) {
        TRAVopt(SETLITERAL_LEFT(node));
    } else {
        TRAVopt(SETLITERAL_RIGHT(node));
    }

    return node;
}


