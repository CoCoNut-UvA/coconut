#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "sets/set.h"


node_st *SLUsetliteral(node_st *node)
{
    struct trav_data_setliteralunion *data = SETLITERALUNION_DATA;
    data->dst = SETIDinsert(data->dst, SETLITERAL_REFERENCE(node));
    TRAVchildren(node);

    return node;
}