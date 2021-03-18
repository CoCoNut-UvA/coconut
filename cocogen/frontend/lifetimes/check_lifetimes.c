/**
 * @file
 *
 * Traversal: checkLifetimes
 * UID      : CLT
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "palm/ctinfo.h"

extern void id_to_info(node_st *ID, struct ctinfo *info);

/**
 * @fn CLTast
 */
node_st *CLTast(node_st *node)
{
    TRAVchildren(node);
    CTIabortOnError();
    return node;
}

/**
 * @fn CLTattribute
 */
node_st *CLTattribute(node_st *node)
{
    if (ATTRIBUTE_LIFETIMES(node) != NULL) {
        if (ATTRIBUTE_TYPE(node) != AT_link && ATTRIBUTE_TYPE(node) != AT_string) {
            struct ctinfo info;
            id_to_info(ATTRIBUTE_NAME(node), &info);
            CTIobj(CTI_ERROR, true, info, "Only node and string attributes can receive a lifetime.");
        }
    }
    TRAVnext(node);
    return node;
}
