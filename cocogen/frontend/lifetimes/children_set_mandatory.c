/**
 * @file
 *
 * Traversal: childrenSetMandatory
 * UID      : CSM
 *
 *
 */

#include <assert.h>
#include <stdio.h>

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"

void CSMinit() { return; }
void CSMfini() { return; }

/**
 * @fn CSMast
 */
node_st *CSMast(node_st *node) {
    DATA_CSM_GET()->ste = AST_STABLE(node);
    TRAVinodes(node);
    return node;
}

/**
 * @fn CSMinode
 */
node_st *CSMinode(node_st *node) {
    TRAVichildren(node);
    TRAVnext(node);
    return node;
}

/**
 * @fn CSMchild
 */
node_st *CSMchild(node_st *node) {
    DATA_CSM_GET()->curr_child = node;
    TRAVlifetimes(node);
    DATA_CSM_GET()->curr_child = NULL;
    TRAVnext(node);
    return node;
}

/**
 * @fn CSMilifetime
 */
node_st *CSMilifetime(node_st *node) {
    assert(DATA_CSM_GET()->curr_child != NULL);
    if (ILIFETIME_TYPE(node) == LT_mandatory) {
        if (ILIFETIME_BEGIN(node) == NULL && ILIFETIME_END(node) == NULL) {
            CHILD_IS_MANDATORY(DATA_CSM_GET()->curr_child) = true;
        }
    }
    TRAVchildren(node);
    return node;
}
