/**
 * @file
 *
 * This traversal assigns a unique ID(integer) to every action.
 * These IDS are used in the check traversal and lifetimes.
 */

#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "frontend/symboltable.h"
#include <stdint.h>


// Start at 2, because 1 is start phase and 0 is undefined.
static uint64_t curr_action_id = 2;
static node_st *ste = NULL;

node_st *AITAast(node_st *node)
{
    ste = AST_STABLE(node);
    TRAVdo(AST_START_PHASE(node));
    return node;
}

node_st *AITAiactions(node_st *node)
{
    IACTIONS_ACTION_ID(node) = curr_action_id++;
    node_st *action = STlookup(ste, IACTIONS_REFERENCE(node));
    if (NODE_TYPE(action) == NT_IPHASE) {
        TRAVopt(IPHASE_IACTIONS(action));
    }
    TRAVopt(IACTIONS_NEXT(node));
    return node;
}
