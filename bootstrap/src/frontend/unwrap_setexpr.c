#include <stddef.h>
#include <stdio.h>
#include "assert.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"

node_st *lookupST(node_st *ste, node_st *node);

node_st *ste;

node_st *USEast(node_st *node)
{
    ste = AST_STABLE(node);
    TRAVchildren(node);
    return node;
}

bool in_set(node_st *node, node_st *val)
{
    if (val == NULL) {
        return false;
    }
    printf("%p\n", SETLITERAL_REFERENCE(node));
    assert(NODE_TYPE(node) == NT_SETLITERAL);
    while(node) {
        if (STReq(ID_ORIG(SETLITERAL_REFERENCE(node)), ID_ORIG(val))) {
            return true;
        }
        node = SETLITERAL_NEXT(node);
    }

    return false;
}

// TODO: rewrite, such that an empty set is denoted by a NULL in the nodeset itself
// instead of having a setliteral with NULL in it.

static node_st *setunion(node_st *node)
{
    node_st *left = SETOPERATION_LEFT(node);
    node_st *curr = SETOPERATION_RIGHT(node);
    node_st *last = NULL;
    node_st *first = NULL;

    if (SETLITERAL_REFERENCE(left) == NULL) {
        return curr;
    }

    while (curr && SETLITERAL_REFERENCE(curr)) {
        if (in_set(left, SETLITERAL_REFERENCE(curr))) {
            node_st *tmp = SETLITERAL_NEXT(curr);
            if (last) {
                SETLITERAL_NEXT(last) = SETLITERAL_NEXT(curr);
            }
            curr = tmp;
        } else {
            printf("%s\n", ID_ORIG(SETLITERAL_REFERENCE(curr)));
            if (first == NULL) {
                first = curr;
            }
            last = curr;
            curr = SETLITERAL_NEXT(curr);
        }
    }
    if (last) {
        SETLITERAL_NEXT(last) = left;
        return first;
    } else {
        return left;
    }
}

static node_st *setintersect(node_st *node)
{
    node_st *left = SETOPERATION_LEFT(node);
    node_st *curr = SETOPERATION_RIGHT(node);
    node_st *last = NULL;
    node_st *first = NULL;

    if (SETLITERAL_REFERENCE(left) == NULL) {
        return ASTsetliteral();
    }

    while (curr && SETLITERAL_REFERENCE(curr)) {
        if (in_set(left, SETLITERAL_REFERENCE(curr))) {
            if (first == NULL) {
                first = curr;
            }
            last = curr;
            curr = SETLITERAL_NEXT(curr);
        } else {
            node_st *tmp = SETLITERAL_NEXT(curr);
            if (last) {
                SETLITERAL_NEXT(last) = SETLITERAL_NEXT(curr);
            }
            curr = tmp;
        }
    }
    if (first == NULL) {
        first = ASTsetliteral();
    }
    return first;
}

static node_st *setdifference(node_st *node)
{
    node_st *curr = SETOPERATION_LEFT(node);
    node_st *right = SETOPERATION_RIGHT(node);
    node_st *first = NULL;
    node_st *last = NULL;

    while (curr) {
        if (!in_set(right, SETLITERAL_REFERENCE(curr))) {
            if (first == NULL) {
                first = curr;
            }
            last = curr;
            curr = SETLITERAL_NEXT(curr);
        } else {
            node_st *tmp = SETLITERAL_NEXT(curr);
            if (last) {
                SETLITERAL_NEXT(last) = SETLITERAL_NEXT(curr);
            }
            curr = tmp;
        }
    }
    if (first == NULL) {
        first = ASTsetliteral();
    }
    return first;
}

node_st *USEsetoperation(node_st *node)
{
    TRAVchildren(node);
    assert(NODE_TYPE(SETOPERATION_LEFT(node)) == NT_SETLITERAL && NODE_TYPE(SETOPERATION_RIGHT(node)) == NT_SETLITERAL);

    switch(SETOPERATION_TYPE(node)) {
    case SO_iunion:
        return setunion(node);
    case SO_intersect:
        return setintersect(node);
    case SO_difference:
        return setdifference(node);
    default:
        assert(false);
    }
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
    printf("TYPE: %d\n", NODE_TYPE(new));
    return new;
}

node_st *USEsetliteral(node_st *node)
{
    TRAVchildren(node);
    return node;
}

