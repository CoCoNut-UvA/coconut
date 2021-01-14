#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "ccngen/trav_data.h"
#include <string.h>
#include <assert.h>

void SLIinit() { return; }
void SLIfini() { return; }


/* Insertion consumes the ID stored in the data struct.
 */
static node_st *NewLiteral(struct data_sli *data)
{
    assert(data->value);
    node_st *new = ASTsetliteral(data->value);
    data->value = NULL;
    data->inserted = true;
    return new;
}

node_st *SLIsetliteral(node_st *node)
{
    struct data_sli *data = DATA_SLI_GET();
    assert(NODE_TYPE(data->value) == NT_ID);
    int cmp = strcmp(ID_LWR(data->value), ID_LWR(SETLITERAL_REFERENCE(node)));

    // Already present, no insertion.
    if (cmp == 0) {
        data->inserted = false;
    } else if (cmp < 0) {
        if (SETLITERAL_LEFT(node)) {
            TRAVdo(SETLITERAL_LEFT(node));
        } else {
            SETLITERAL_LEFT(node) = NewLiteral(data);
        }
    } else {
        if (SETLITERAL_RIGHT(node)) {
            TRAVdo(SETLITERAL_RIGHT(node));
        } else {
            SETLITERAL_RIGHT(node) = NewLiteral(data);
        }
    }

    return node;
}
