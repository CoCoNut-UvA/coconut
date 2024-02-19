/**
 * @file
 *
 * Traversal: checkAttributes
 * UID      : CHA
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "frontend/ctihelp.h"
#include "palm/ctinfo.h"

void CHAinit() { return; }
void CHAfini() { return; }

/**
 * @fn CHAinodeset
 */
node_st *CHAinodeset(node_st *node) {
    DATA_CHA_GET()->in_nodeset = true;
    if (INODESET_ILLEGAL_SETEXPR_ATTR(node) && INODESET_IATTRIBUTES(node)) {
        struct ctinfo info;
        id_to_info(INODESET_NAME(node), &info);
        CTIobj(CTI_ERROR, true, info,
               "Cannot use intersect (&) and difference (-) operators in set "
               "expressions for nodesets with associated attributes\n");
        CCNerrorPhase();
    }

    TRAViattributes(node);
    TRAVnext(node);

    return node;
}

/**
 * @fn CHAinode
 */
node_st *CHAinode(node_st *node) {
    DATA_CHA_GET()->in_nodeset = false;
    DATA_CHA_GET()->curr_node = node;

    TRAViattributes(node);
    TRAVnext(node);

    return node;
}

/**
 * @fn CHAattribute
 */
node_st *CHAattribute(node_st *node) {
    struct ctinfo info;
    id_to_info(ATTRIBUTE_NAME(node), &info);

    if (ATTRIBUTE_IS_INHERITED(node) && !(DATA_CHA_GET()->in_nodeset) &&
        INODE_IS_ROOT(DATA_CHA_GET()->curr_node)) {
            CTIobj(
                CTI_ERROR, true, info,
                "The root node '%s' cannot have inherited attributes\n",
                ID_ORIG(INODE_NAME(DATA_CHA_GET()->curr_node)));
            CCNerrorPhase();
    }

    // Don't check propagated attributes for the rest to prevent duplicate
    // errors
    if (ATTRIBUTE_IS_PROPAGATED(node)) {
        return node;
    }

    if (ATTRIBUTE_IN_CONSTRUCTOR(node)) {
        if (DATA_CHA_GET()->in_nodeset) {
            CTIobj(CTI_ERROR, true, info,
                   "Nodeset attributes cannot use the constructor keyword\n");
            CCNerrorAction();
        } else if (ATTRIBUTE_IS_INHERITED(node) ||
                   ATTRIBUTE_IS_SYNTHESIZED(node)) {
            CTIobj(CTI_ERROR, true, info,
                   "Inherited or synthesized attributes cannot use the "
                   "constructor keyword\n");
            CCNerrorPhase();
        }
    }

    if (ATTRIBUTE_LIFETIMES(node)) {
        if (ATTRIBUTE_IS_INHERITED(node) || ATTRIBUTE_IS_SYNTHESIZED(node)) {
            CTIobj(
                CTI_ERROR, true, info,
                "Inherited or synthesized attributes cannot use lifetimes\n");
            CCNerrorPhase();
        }
    }

    return node;
}
