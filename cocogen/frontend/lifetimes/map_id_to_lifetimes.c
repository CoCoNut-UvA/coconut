/**
 * @file
 *
 * This traversal maps lifetime targets to the action ID the target belongs
 * to.
 */

#include <palm/ctinfo.h>
#include <palm/str.h>
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "frontend/symboltable.h"

extern void id_to_info(node_st *ID, struct ctinfo *info);
static node_st *last_action = NULL;
static node_st *last_node = NULL;
static node_st *curr_target = NULL;
static node_st *ast = NULL;
static node_st *ste = NULL;
static bool in_node = false;
static bool in_lifetime = false;

/**
 * Compare if two identifiers are equal by comparing their lower case form.
 * @param id
 * @param other_id
 * @return lower(id) == lower(other_id)
 */
static bool CompareID(node_st *id, node_st *other_id)
{
    if (id == NULL || other_id == NULL) {
        return false;
    }
    return STReq(ID_LWR(id), ID_LWR(other_id));
}


node_st *MITLast(node_st *node)
{
    ast = node;
    ste = AST_STABLE(node);
    TRAVopt(AST_INODES(node));
    CTIabortOnError();
    return node;
}

node_st *MITLinode(node_st *node)
{
    // Node lifetimes have special requirements(less options), so denote
    // we are in a node.
    in_node = true;
    TRAVopt(INODE_LIFETIMES(node));
    in_node = false;
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_IATTRIBUTES(node));
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *MITLiactions(node_st *node)
{
    node_st *action_lookup = STlookup(ste, IACTIONS_REFERENCE(node));
    if (action_lookup == NULL) {
        CTI(CTI_ERROR, true, "Lifetime target(%s) does not reference a valid action.", ID_ORIG(IACTIONS_REFERENCE(node)));
    }
    if (NODE_TYPE(action_lookup) == NT_IPHASE) {
        if (CompareID(IPHASE_NAME(action_lookup), curr_target) || CompareID(IPHASE_IPREFIX(action_lookup), curr_target)) {
            last_action = node;
            last_node = action_lookup;
            return node;
        }
        TRAVopt(IPHASE_IACTIONS(action_lookup));
    } else if (NODE_TYPE(action_lookup) == NT_IPASS) {
        if (CompareID(IPASS_NAME(action_lookup), curr_target) || CompareID(IPASS_IPREFIX(action_lookup), curr_target)) {
            last_action = node;
            last_node = action_lookup;
            return node;
        }
    } else if (NODE_TYPE(action_lookup) == NT_ITRAVERSAL) {
        if (CompareID(ITRAVERSAL_NAME(action_lookup), curr_target) || CompareID(ITRAVERSAL_IPREFIX(action_lookup), curr_target)) {
            last_node = action_lookup;
            last_action = node;
            return node;
        }
    }
    if (!last_action) {
        TRAVopt(IACTIONS_NEXT(node));
    }
    return node;
}

node_st *MITLilifetime(node_st *node)
{
    if (in_node && (ILIFETIME_TYPE(node) == LT_mandatory || ILIFETIME_TYPE(node) == LT_optional)) {
        CTI(CTI_ERROR, true, "Node lifetime can not use mandatory/optional.");
    }
    last_action = NULL;
    TRAVopt(ILIFETIME_BEGIN(node));
    last_action = NULL;
    TRAVopt(ILIFETIME_END(node));
    return node;
}


node_st *MITLlifetime_range(node_st *node)
{
    curr_target = LIFETIME_RANGE_TARGET(node);
    in_lifetime = true;
    TRAVopt(LIFETIME_RANGE_TARGET(node));
    if (!last_action) {
        struct ctinfo info;
        id_to_info(LIFETIME_RANGE_TARGET(node), &info);
        info.last_column = 0;
        CTIobj(CTI_ERROR, true, info, "Lifetime does not target a valid action.");
    } else {
        LIFETIME_RANGE_ACTION_ID(node) = IACTIONS_ACTION_ID(last_action);
        if (NODE_TYPE(last_node) == NT_IPHASE) {
            node_st *phase_actions = IPHASE_IACTIONS(last_node);
            node_st *next = IACTIONS_NEXT(phase_actions);
            while (next) {
                phase_actions = next;
                next = IACTIONS_NEXT(phase_actions);
            }
            LIFETIME_RANGE_NEXT_ACTION_ID(node) = IACTIONS_ACTION_ID(phase_actions) + 1;
        } else {
            LIFETIME_RANGE_NEXT_ACTION_ID(node) = IACTIONS_ACTION_ID(last_action) + 1;
        }
    }
    in_lifetime = false;
    curr_target = NULL;
    return node;
}

node_st *MITLid(node_st *node)
{
    if (!in_lifetime) {
        return node;
    }
    curr_target = node;
    if (last_action == NULL) {
        TRAVopt(IPHASE_IACTIONS(AST_START_PHASE(ast)));
    } else {
        node_st *action = STlookup(ste, IACTIONS_REFERENCE(last_action));
        if (!action || NODE_TYPE(action) != NT_IPHASE)  {
            struct ctinfo info;
            id_to_info(node, &info);
            // Only use the '^' marker and not '~~~'.
            info.last_column = 0;
            // Align marker on the dot.
            if (info.first_column > 0) {
                info.first_column -= 1;
            }
            CTIobj(CTI_ERROR, true, info, "Lifetime does not target a phase while it nests.");
        } else {
            last_action = NULL;
            last_node = NULL;
            TRAVopt(IPHASE_IACTIONS(action));
        }
    }
    if (last_action) {
        TRAVopt(ID_NEXT(curr_target));
    }
    return node;
}
