#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "sets/set.h"

void SLISinit() { return; }
void SLISfini() { return; }


node_st *SLISsetliteral(node_st *node)
{
    struct data_slis *data = DATA_SLIS_GET();
    if (SETIDcontains(data->src, SETLITERAL_REFERENCE(node))) {
        data->new = SETIDinsert(data->new, SETLITERAL_REFERENCE(node));
    }
    TRAVchildren(node);

    return node;
}
