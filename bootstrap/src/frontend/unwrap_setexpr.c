#include <stddef.h>
#include <stdio.h>
#include "assert.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "sets/set.h"

node_st *lookupST(node_st *ste, node_st *node);

node_st *ste;

node_st *USEast(node_st *node)
{
    ste = AST_STABLE(node);
    TRAVchildren(node);
    return node;
}

node_st *USEsetoperation(node_st *node)
{
    TRAVchildren(node);
    assert(NODE_TYPE(SETOPERATION_LEFT(node)) == NT_SETLITERAL && NODE_TYPE(SETOPERATION_RIGHT(node)) == NT_SETLITERAL);
    node_st *new = NULL;
    switch(SETOPERATION_TYPE(node)) {
    case SO_iunion:
        new = SETIDunion(SETOPERATION_LEFT(node), SETOPERATION_RIGHT(node));
        break;
    case SO_intersect:
        new = SETIDintersect(SETOPERATION_LEFT(node), SETOPERATION_RIGHT(node));
        break;
    case SO_difference:
        //return setdifference(node);
    default:
        assert(false);
    }
    TRAVstart(node, TRAV_free);
    return new;
}

node_st *USEsetreference(node_st *node)
{
    node_st *nodeset = lookupST(ste, SETREFERENCE_REFERENCE(node));
    assert(nodeset && NODE_TYPE(nodeset) == NT_INODESET);
    if (NODE_TYPE(INODESET_EXPR(nodeset)) != NT_SETLITERAL) {
        TRAVchildren(nodeset);
    }
    if (NODE_TYPE(INODESET_EXPR(nodeset)) != NT_SETLITERAL) {
        assert(false);
    }
    node_st *curr = INODESET_EXPR(nodeset);
    node_st *new = TRAVstart(curr, TRAV_cpy);
    TRAVstart(node, TRAV_free);
    return new;
}

node_st *USEsetliteral(node_st *node)
{
    TRAVchildren(node);
    return node;
}

