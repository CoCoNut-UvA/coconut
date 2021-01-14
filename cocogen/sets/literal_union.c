#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "sets/set.h"

void SLUinit() { return; }
void SLUfini() { return; }


node_st *SLUsetliteral(node_st *node)
{
    struct data_slu *data = DATA_SLU_GET();
    data->dst = SETIDinsert(data->dst, SETLITERAL_REFERENCE(node));
    TRAVchildren(node);

    return node;
}
