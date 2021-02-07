/**
 * @file
 *
 * This traversal unwraps set expressions such that all operations become
 * set literals.
 */

#include <stddef.h>
#include "assert.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "sets/set.h"
#include "frontend/symboltable.h"

static node_st *ste;

node_st *USEast(node_st *node)
{
    ste = AST_STABLE(node);
    TRAVchildren(node);
    return node;
}

node_st *USEsetoperation(node_st *node)
{
    TRAVchildren(node);
    if (SETOPERATION_LEFT(node)) {
        assert(NODE_TYPE(SETOPERATION_LEFT(node)) == NT_SETLITERAL);
    }
    if (SETOPERATION_RIGHT(node)) {
        assert(NODE_TYPE(SETOPERATION_RIGHT(node)) == NT_SETLITERAL);
    }
    node_st *new = NULL;
    switch(SETOPERATION_TYPE(node)) {
    case SO_iunion:
        new = SETIDunion(SETOPERATION_LEFT(node), SETOPERATION_RIGHT(node));
        break;
    case SO_intersect:
        new = SETIDintersect(SETOPERATION_LEFT(node), SETOPERATION_RIGHT(node));
        break;
    case SO_difference:
        new = SETIDdifference(SETOPERATION_LEFT(node), SETOPERATION_RIGHT(node));
        break;
    default:
        assert(false);
    }
    TRAVstart(node, TRAV_free);
    return new;
}

/**
 * Unpacks a set reference by looking up the corresponding nodeset,
 * evaluating the nodeset and returning the resulting set literal.
 * @param node Setreference that is freed after this traversal.
 * @return
 */
node_st *USEsetreference(node_st *node)
{
    assert(node);
    node_st *nodeset = STlookup(ste, SETREFERENCE_REFERENCE(node));
    assert(nodeset && NODE_TYPE(nodeset) == NT_INODESET);
    if (INODESET_EXPR(nodeset) && NODE_TYPE(INODESET_EXPR(nodeset)) != NT_SETLITERAL) {
        TRAVchildren(nodeset);
    }
    if (INODESET_EXPR(nodeset)) {
        assert(NODE_TYPE(INODESET_EXPR(nodeset)) == NT_SETLITERAL);
        if (NODE_TYPE(INODESET_EXPR(nodeset)) != NT_SETLITERAL) {
            assert(false);
        }
    } else {
        TRAVstart(node, TRAV_free);
        return NULL;
    }
    node_st *curr = INODESET_EXPR(nodeset);
    node_st *new = TRAVstart(curr, TRAV_cpy);
    TRAVstart(node, TRAV_free);
    return new;
}

node_st *USEsetliteral(node_st *node)
{
    return node;
}

