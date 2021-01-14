#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "sets/set.h"

void SLDinit() {return; }
void SLDfini() {return; }

node_st *SLDsetliteral(node_st *node)
{
    struct data_sld *data = DATA_SLD_GET();
    if(!SETIDcontains(data->right, SETLITERAL_REFERENCE(node))) {
        data->new = SETIDinsert(data->new, SETLITERAL_REFERENCE(node));
    }
    TRAVchildren(node);
    return node;
}
