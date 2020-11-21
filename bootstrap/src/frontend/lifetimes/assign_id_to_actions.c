#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include <stdint.h>

uint64_t curr_action_id = 1;

node_st *AITAast(node_st *node)
{
    TRAVdo(AST_ROOT_NODE(node));
    return node;
}

node_st *AITAiactions(node_st *node)
{
    IACTIONS_ACTION_ID(node) = curr_action_id++;
    TRAVchildren(node);
    return node;
}