#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "sets/set.h"


node_st *SLDsetliteral(node_st *node)
{
    struct trav_data_setliteraldifference *data = SETLITERALDIFFERENCE_DATA;
    if(!SETIDcontains(data->right, SETLITERAL_REFERENCE(node))) {
        data->new = SETIDinsert(data->new, SETLITERAL_REFERENCE(node));
    }
    TRAVchildren(node);
    return node;
}