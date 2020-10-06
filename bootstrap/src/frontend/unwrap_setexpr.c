#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"


void *USEallocTravData()
{
    return 0;
}

void USEfreeTravData(void *data)
{

}



node_st *lookupST(node_st *ste, node_st *node);

node_st *ste;

node_st *USEast(node_st *node)
{
    ste = AST_STABLE(node);
    TRAVchildren(node);
    return node;
}

node_st *USEiactions(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *USEiphas(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *USEitraversal(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *USEipass(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *USEinode(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *USEinodeset(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *USEchild(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *USEste(node_st *node)
{
    TRAVchildren(node);
    return node;
}

struct ccn_node *cpyid(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTnewid(NULL, NULL, NULL    );
    ID_ORIG(new_node) = STRcpy(ID_ORIG(arg_node));
    ID_LWR(new_node) = STRcpy(ID_LWR(arg_node));
    ID_UPR(new_node) = STRcpy(ID_UPR(arg_node));
    if (ID_NEXT(new_node)) {
        ID_NEXT(new_node) = cpyid(ID_NEXT(arg_node));
    }
    return new_node;
}

struct ccn_node *cpysetliteral(node_st *node)
{
    node_st *new;
    node_st *first = NULL;
    node_st *last = NULL;
    while (node) {
        new = ASTnewsetliteral();
        SETLITERAL_REFERENCE(new) = cpyid(SETLITERAL_REFERENCE(node));
        if (first == NULL) {
            first = new;
            last = new;
        } else {
            SETLITERAL_NEXT(last) = new;
            last = new;
        }
        node = SETLITERAL_NEXT(node);
    }

    return first;
}

bool in_set(node_st *node, node_st *val)
{
    assert(NODE_TYPE(node) == NT_SETLITERAL);
    while(node) {
        if (STReq(ID_ORIG(SETLITERAL_REFERENCE(node)), ID_ORIG(val))) {
            return true;
        }
        node = SETLITERAL_NEXT(node);
    }

    return false;
}

static node_st *setunion(node_st *node)
{
    node_st *left = SETOPERATION_LEFT(node);
    node_st *curr = SETOPERATION_RIGHT(node);
    node_st *last = NULL;
    node_st *first = NULL;

    while (curr) {
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

    while (curr) {
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
        first = ASTnewsetliteral();
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
    node_st *new = cpysetliteral(curr);
    printf("TYPE: %d\n", NODE_TYPE(new));
    return new;
}

node_st *USEsetliteral(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *USEattribute(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *USEienum(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *USEid(node_st *node)
{

    TRAVchildren(node);
    return node;
}


node_st *USEiphase(node_st *node)
{
    TRAVchildren(node);
    return node;
}


