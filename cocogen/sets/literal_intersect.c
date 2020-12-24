#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "sets/set.h"


node_st *SLISsetliteral(node_st *node)
{
    struct trav_data_setliteralintersect *data = SETLITERALINTERSECT_DATA;
    if (SETIDcontains(data->src, SETLITERAL_REFERENCE(node))) {
        data->new = SETIDinsert(data->new, SETLITERAL_REFERENCE(node));
    }
    TRAVchildren(node);

    return node;
}