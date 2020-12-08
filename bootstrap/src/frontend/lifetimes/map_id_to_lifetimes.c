#include <palm/ctinfo.h>
#include <palm/str.h>
#include "ccngen/ast.h"
#include "ccn/dynamic_core.h"
#include "palm/hash_table.h"

extern node_st *lookupST(node_st*, node_st*);

static node_st *last_action = NULL;
static node_st *curr_target = NULL;
static node_st *ast = NULL;
static node_st *ste = NULL;
static bool in_node = false;

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
    in_node = true;
    TRAVopt(INODE_LIFETIMES(node));
    in_node = false;
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_NEXT(node));
    return node;
}

node_st *MITLiactions(node_st *node)
{
    node_st *action_lookup = lookupST(ste, IACTIONS_REFERENCE(node));
    if (action_lookup == NULL) {
        CTIerror("Lifetime target does not reference a valid action.");
    }
    if (NODE_TYPE(action_lookup) == NT_IPHASE) {
        if (CompareID(IPHASE_NAME(action_lookup), curr_target) || CompareID(IPHASE_IPREFIX(action_lookup), curr_target)) {
            last_action = node;
            return node;
        }
        TRAVopt(IPHASE_IACTIONS(action_lookup));
    } else if (NODE_TYPE(action_lookup) == NT_IPASS) {
        if (CompareID(IPASS_NAME(action_lookup), curr_target) || CompareID(IPASS_IPREFIX(action_lookup), curr_target)) {
            last_action = node;
            return node;
        }
    } else if (NODE_TYPE(action_lookup) == NT_ITRAVERSAL) {
        if (CompareID(ITRAVERSAL_NAME(action_lookup), curr_target) || CompareID(ITRAVERSAL_IPREFIX(action_lookup), curr_target)) {
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
    if (in_node && ILIFETIME_TYPE(node) == LT_mandatory) {
        CTIerror("Node lifetime can not use mandatory.");
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
    TRAVopt(LIFETIME_RANGE_TARGET(node));
    if (!last_action) {
        CTIerror("Lifetime does not target a valid action.");
    } else {
        LIFETIME_RANGE_ACTION_ID(node) = IACTIONS_ACTION_ID(last_action);
    }
    curr_target = NULL;
    return node;
}

node_st *MITLid(node_st *node)
{
    curr_target = node;
    if (last_action == NULL) {
        TRAVopt(IPHASE_IACTIONS(AST_START_PHASE(ast)));
    } else {
        node_st *action = lookupST(ste, IACTIONS_REFERENCE(last_action));
        if (!action || NODE_TYPE(action) != NT_IPHASE)  {
            CTIerror("Lifetime does not target a phase while it nests.");
        } else {
            last_action = NULL;
            TRAVopt(IPHASE_IACTIONS(action));
        }
    }
    TRAVopt(ID_NEXT(curr_target));
    return node;
}