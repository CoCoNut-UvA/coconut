#include <stdbool.h>
#include "ccn/dynamic_core.h"
#include "ccn/phase_driver.h"
#include "ccngen/ast.h"
#include "palm/ctinfo.h"
char *nodetypeToName(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_ID:
            return "id";
            break;
        case NT_IENUM:
            return "ienum";
            break;
        case NT_ATTRIBUTE:
            return "attribute";
            break;
        case NT_ITRAVDATA:
            return "itravdata";
            break;
        case NT_SETOPERATION:
            return "setoperation";
            break;
        case NT_SETLITERAL:
            return "setliteral";
            break;
        case NT_SETREFERENCE:
            return "setreference";
            break;
        case NT_STE:
            return "ste";
            break;
        case NT_CHILD:
            return "child";
            break;
        case NT_LIFETIME_RANGE:
            return "lifetime_range";
            break;
        case NT_ILIFETIME:
            return "ilifetime";
            break;
        case NT_INODESET:
            return "inodeset";
            break;
        case NT_INODE:
            return "inode";
            break;
        case NT_IPASS:
            return "ipass";
            break;
        case NT_ITRAVERSAL:
            return "itraversal";
            break;
        case NT_IPHASE:
            return "iphase";
            break;
        case NT_IACTIONS:
            return "iactions";
            break;
        case NT_AST:
            return "ast";
            break;
        default:
            return "Unknown";
                }

}

static bool TypeIssetexpr(node_st *arg_node) {
    enum ccn_nodetype node_type = NODE_TYPE(arg_node);
    return (false || node_type == NT_SETOPERATION || node_type == NT_SETLITERAL || node_type == NT_SETREFERENCE    );
}

static bool TypeIslink(node_st *arg_node) {
    enum ccn_nodetype node_type = NODE_TYPE(arg_node);
    return (false || node_type == NT_IPHASE || node_type == NT_IPASS || node_type == NT_INODE || node_type == NT_ID || node_type == NT_ITRAVERSAL    );
}

static bool TypeIsaction(node_st *arg_node) {
    enum ccn_nodetype node_type = NODE_TYPE(arg_node);
    return (false || node_type == NT_IPHASE || node_type == NT_IPASS || node_type == NT_ITRAVERSAL    );
}

struct ccn_node *CHKid(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (ID_NEXT(arg_node)) {
        if (NODE_TYPE(ID_NEXT(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(id) has disallowed type(%s) ", nodetypeToName(ID_NEXT(arg_node)));
        }

    }

    if (ID_ORIG(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Attribute(orig) in node(id) is missing, but specified as mandatory.\n");;
    }

    if (ID_LWR(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Attribute(lwr) in node(id) is missing, but specified as mandatory.\n");;
    }

    if (ID_UPR(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Attribute(Upr) in node(id) is missing, but specified as mandatory.\n");;
    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKienum(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (IENUM_VALS(arg_node)) {
        if (NODE_TYPE(IENUM_VALS(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(Vals) of node(ienum) has disallowed type(%s) ", nodetypeToName(IENUM_VALS(arg_node)));
        }

    }

    if (IENUM_NAME(arg_node)) {
        if (NODE_TYPE(IENUM_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(ienum) has disallowed type(%s) ", nodetypeToName(IENUM_NAME(arg_node)));
        }

    }

    if (IENUM_IPREFIX(arg_node)) {
        if (NODE_TYPE(IENUM_IPREFIX(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(iprefix) of node(ienum) has disallowed type(%s) ", nodetypeToName(IENUM_IPREFIX(arg_node)));
        }

    }

    if (IENUM_NEXT(arg_node)) {
        if (NODE_TYPE(IENUM_NEXT(arg_node)) != NT_IENUM) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(ienum) has disallowed type(%s) ", nodetypeToName(IENUM_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKattribute(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (ATTRIBUTE_NAME(arg_node)) {
        if (NODE_TYPE(ATTRIBUTE_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(attribute) has disallowed type(%s) ", nodetypeToName(ATTRIBUTE_NAME(arg_node)));
        }

    }

    if (ATTRIBUTE_NAME(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Child(name) in node(attribute) is missing, but specified as mandatory.\n");;
    }

    if (ATTRIBUTE_TYPE_REFERENCE(arg_node)) {
        if (NODE_TYPE(ATTRIBUTE_TYPE_REFERENCE(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(type_reference) of node(attribute) has disallowed type(%s) ", nodetypeToName(ATTRIBUTE_TYPE_REFERENCE(arg_node)));
        }

    }

    if (ATTRIBUTE_LIFETIMES(arg_node)) {
        if (NODE_TYPE(ATTRIBUTE_LIFETIMES(arg_node)) != NT_ILIFETIME) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(lifetimes) of node(attribute) has disallowed type(%s) ", nodetypeToName(ATTRIBUTE_LIFETIMES(arg_node)));
        }

    }

    if (ATTRIBUTE_NEXT(arg_node)) {
        if (NODE_TYPE(ATTRIBUTE_NEXT(arg_node)) != NT_ATTRIBUTE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(attribute) has disallowed type(%s) ", nodetypeToName(ATTRIBUTE_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKitravdata(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (ITRAVDATA_NAME(arg_node)) {
        if (NODE_TYPE(ITRAVDATA_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(itravdata) has disallowed type(%s) ", nodetypeToName(ITRAVDATA_NAME(arg_node)));
        }

    }

    if (ITRAVDATA_NAME(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Child(name) in node(itravdata) is missing, but specified as mandatory.\n");;
    }

    if (ITRAVDATA_TYPE_REFERENCE(arg_node)) {
        if (NODE_TYPE(ITRAVDATA_TYPE_REFERENCE(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(type_reference) of node(itravdata) has disallowed type(%s) ", nodetypeToName(ITRAVDATA_TYPE_REFERENCE(arg_node)));
        }

    }

    if (ITRAVDATA_NEXT(arg_node)) {
        if (NODE_TYPE(ITRAVDATA_NEXT(arg_node)) != NT_ITRAVDATA) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(itravdata) has disallowed type(%s) ", nodetypeToName(ITRAVDATA_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKsetoperation(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (SETOPERATION_LEFT(arg_node)) {
        if (!TypeIssetexpr(SETOPERATION_LEFT(arg_node))) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(left) of node(setoperation) has disallowed type(%s) ", nodetypeToName(SETOPERATION_LEFT(arg_node)));
        }

    }

    if (SETOPERATION_LEFT(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Child(left) in node(setoperation) is missing, but specified as mandatory.\n");;
    }

    if (SETOPERATION_RIGHT(arg_node)) {
        if (!TypeIssetexpr(SETOPERATION_RIGHT(arg_node))) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(right) of node(setoperation) has disallowed type(%s) ", nodetypeToName(SETOPERATION_RIGHT(arg_node)));
        }

    }

    if (SETOPERATION_RIGHT(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Child(right) in node(setoperation) is missing, but specified as mandatory.\n");;
    }

    if (action_id >= 8 && true) {
        CTI(CTI_ERROR, true, "Found disallowed node(setoperation) in tree.\n");
    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKsetliteral(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (SETLITERAL_REFERENCE(arg_node)) {
        if (NODE_TYPE(SETLITERAL_REFERENCE(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(reference) of node(setliteral) has disallowed type(%s) ", nodetypeToName(SETLITERAL_REFERENCE(arg_node)));
        }

    }

    if (SETLITERAL_LEFT(arg_node)) {
        if (NODE_TYPE(SETLITERAL_LEFT(arg_node)) != NT_SETLITERAL) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(left) of node(setliteral) has disallowed type(%s) ", nodetypeToName(SETLITERAL_LEFT(arg_node)));
        }

    }

    if (SETLITERAL_RIGHT(arg_node)) {
        if (NODE_TYPE(SETLITERAL_RIGHT(arg_node)) != NT_SETLITERAL) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(right) of node(setliteral) has disallowed type(%s) ", nodetypeToName(SETLITERAL_RIGHT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKsetreference(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (SETREFERENCE_REFERENCE(arg_node)) {
        if (NODE_TYPE(SETREFERENCE_REFERENCE(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(reference) of node(setreference) has disallowed type(%s) ", nodetypeToName(SETREFERENCE_REFERENCE(arg_node)));
        }

    }

    if (action_id >= 8 || false) {
        CTI(CTI_ERROR, true, "Found disallowed Found disallowed node(setreference) in tree. in tree.\n");
    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKste(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (STE_NEXT(arg_node)) {
        if (NODE_TYPE(STE_NEXT(arg_node)) != NT_STE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(ste) has disallowed type(%s) ", nodetypeToName(STE_NEXT(arg_node)));
        }

    }

    if (action_id >= 5 && true) {
        if (STE_VALUE(arg_node) == NULL) {
            CTI(CTI_ERROR, true, "Mandatory attribute(value) in node(ste) is missing.\n");;
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKchild(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (CHILD_NAME(arg_node)) {
        if (NODE_TYPE(CHILD_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(child) has disallowed type(%s) ", nodetypeToName(CHILD_NAME(arg_node)));
        }

    }

    if (CHILD_NAME(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Child(name) in node(child) is missing, but specified as mandatory.\n");;
    }

    if (CHILD_LIFETIMES(arg_node)) {
        if (NODE_TYPE(CHILD_LIFETIMES(arg_node)) != NT_ILIFETIME) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(lifetimes) of node(child) has disallowed type(%s) ", nodetypeToName(CHILD_LIFETIMES(arg_node)));
        }

    }

    if (CHILD_NEXT(arg_node)) {
        if (NODE_TYPE(CHILD_NEXT(arg_node)) != NT_CHILD) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(child) has disallowed type(%s) ", nodetypeToName(CHILD_NEXT(arg_node)));
        }

    }

    if (CHILD_TYPE_REFERENCE(arg_node)) {
        if (NODE_TYPE(CHILD_TYPE_REFERENCE(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(type_reference) of node(child) has disallowed type(%s) ", nodetypeToName(CHILD_TYPE_REFERENCE(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKlifetime_range(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (LIFETIME_RANGE_TARGET(arg_node)) {
        if (NODE_TYPE(LIFETIME_RANGE_TARGET(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(target) of node(lifetime_range) has disallowed type(%s) ", nodetypeToName(LIFETIME_RANGE_TARGET(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKilifetime(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (ILIFETIME_BEGIN(arg_node)) {
        if (NODE_TYPE(ILIFETIME_BEGIN(arg_node)) != NT_LIFETIME_RANGE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(begin) of node(ilifetime) has disallowed type(%s) ", nodetypeToName(ILIFETIME_BEGIN(arg_node)));
        }

    }

    if (ILIFETIME_END(arg_node)) {
        if (NODE_TYPE(ILIFETIME_END(arg_node)) != NT_LIFETIME_RANGE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(end) of node(ilifetime) has disallowed type(%s) ", nodetypeToName(ILIFETIME_END(arg_node)));
        }

    }

    if (ILIFETIME_NEXT(arg_node)) {
        if (NODE_TYPE(ILIFETIME_NEXT(arg_node)) != NT_ILIFETIME) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(ilifetime) has disallowed type(%s) ", nodetypeToName(ILIFETIME_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKinodeset(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (INODESET_NAME(arg_node)) {
        if (NODE_TYPE(INODESET_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(inodeset) has disallowed type(%s) ", nodetypeToName(INODESET_NAME(arg_node)));
        }

    }

    if (INODESET_EXPR(arg_node)) {
        if (!TypeIssetexpr(INODESET_EXPR(arg_node))) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(expr) of node(inodeset) has disallowed type(%s) ", nodetypeToName(INODESET_EXPR(arg_node)));
        }

    }

    if (INODESET_UNPACKED(arg_node)) {
        if (NODE_TYPE(INODESET_UNPACKED(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(unpacked) of node(inodeset) has disallowed type(%s) ", nodetypeToName(INODESET_UNPACKED(arg_node)));
        }

    }

    if (INODESET_NEXT(arg_node)) {
        if (NODE_TYPE(INODESET_NEXT(arg_node)) != NT_INODESET) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(inodeset) has disallowed type(%s) ", nodetypeToName(INODESET_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKinode(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (INODE_NAME(arg_node)) {
        if (NODE_TYPE(INODE_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(inode) has disallowed type(%s) ", nodetypeToName(INODE_NAME(arg_node)));
        }

    }

    if (INODE_NAME(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Child(name) in node(inode) is missing, but specified as mandatory.\n");;
    }

    if (INODE_NEXT(arg_node)) {
        if (NODE_TYPE(INODE_NEXT(arg_node)) != NT_INODE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(inode) has disallowed type(%s) ", nodetypeToName(INODE_NEXT(arg_node)));
        }

    }

    if (INODE_ICHILDREN(arg_node)) {
        if (NODE_TYPE(INODE_ICHILDREN(arg_node)) != NT_CHILD) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(ichildren) of node(inode) has disallowed type(%s) ", nodetypeToName(INODE_ICHILDREN(arg_node)));
        }

    }

    if (INODE_IATTRIBUTES(arg_node)) {
        if (NODE_TYPE(INODE_IATTRIBUTES(arg_node)) != NT_ATTRIBUTE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(iattributes) of node(inode) has disallowed type(%s) ", nodetypeToName(INODE_IATTRIBUTES(arg_node)));
        }

    }

    if (INODE_LIFETIMES(arg_node)) {
        if (NODE_TYPE(INODE_LIFETIMES(arg_node)) != NT_ILIFETIME) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(lifetimes) of node(inode) has disallowed type(%s) ", nodetypeToName(INODE_LIFETIMES(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKipass(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (IPASS_NAME(arg_node)) {
        if (NODE_TYPE(IPASS_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(ipass) has disallowed type(%s) ", nodetypeToName(IPASS_NAME(arg_node)));
        }

    }

    if (IPASS_IPREFIX(arg_node)) {
        if (NODE_TYPE(IPASS_IPREFIX(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(iprefix) of node(ipass) has disallowed type(%s) ", nodetypeToName(IPASS_IPREFIX(arg_node)));
        }

    }

    if (IPASS_TARGET_FUNC(arg_node)) {
        if (NODE_TYPE(IPASS_TARGET_FUNC(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(target_func) of node(ipass) has disallowed type(%s) ", nodetypeToName(IPASS_TARGET_FUNC(arg_node)));
        }

    }

    if (IPASS_NEXT(arg_node)) {
        if (NODE_TYPE(IPASS_NEXT(arg_node)) != NT_IPASS) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(ipass) has disallowed type(%s) ", nodetypeToName(IPASS_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKitraversal(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (ITRAVERSAL_NAME(arg_node)) {
        if (NODE_TYPE(ITRAVERSAL_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(itraversal) has disallowed type(%s) ", nodetypeToName(ITRAVERSAL_NAME(arg_node)));
        }

    }

    if (ITRAVERSAL_IPREFIX(arg_node)) {
        if (NODE_TYPE(ITRAVERSAL_IPREFIX(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(iprefix) of node(itraversal) has disallowed type(%s) ", nodetypeToName(ITRAVERSAL_IPREFIX(arg_node)));
        }

    }

    if (ITRAVERSAL_INODES(arg_node)) {
        if (!TypeIssetexpr(ITRAVERSAL_INODES(arg_node))) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(inodes) of node(itraversal) has disallowed type(%s) ", nodetypeToName(ITRAVERSAL_INODES(arg_node)));
        }

    }

    if (ITRAVERSAL_DATA(arg_node)) {
        if (NODE_TYPE(ITRAVERSAL_DATA(arg_node)) != NT_ITRAVDATA) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(data) of node(itraversal) has disallowed type(%s) ", nodetypeToName(ITRAVERSAL_DATA(arg_node)));
        }

    }

    if (ITRAVERSAL_NEXT(arg_node)) {
        if (NODE_TYPE(ITRAVERSAL_NEXT(arg_node)) != NT_ITRAVERSAL) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(itraversal) has disallowed type(%s) ", nodetypeToName(ITRAVERSAL_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKiphase(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (IPHASE_NAME(arg_node)) {
        if (NODE_TYPE(IPHASE_NAME(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(name) of node(iphase) has disallowed type(%s) ", nodetypeToName(IPHASE_NAME(arg_node)));
        }

    }

    if (IPHASE_IPREFIX(arg_node)) {
        if (NODE_TYPE(IPHASE_IPREFIX(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(iprefix) of node(iphase) has disallowed type(%s) ", nodetypeToName(IPHASE_IPREFIX(arg_node)));
        }

    }

    if (IPHASE_GATE_FUNC(arg_node)) {
        if (NODE_TYPE(IPHASE_GATE_FUNC(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(gate_func) of node(iphase) has disallowed type(%s) ", nodetypeToName(IPHASE_GATE_FUNC(arg_node)));
        }

    }

    if (IPHASE_IACTIONS(arg_node)) {
        if (NODE_TYPE(IPHASE_IACTIONS(arg_node)) != NT_IACTIONS) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(iactions) of node(iphase) has disallowed type(%s) ", nodetypeToName(IPHASE_IACTIONS(arg_node)));
        }

    }

    if (IPHASE_IACTIONS(arg_node) == NULL) {
        CTI(CTI_ERROR, true, "Child(iactions) in node(iphase) is missing, but specified as mandatory.\n");;
    }

    if (IPHASE_NEXT(arg_node)) {
        if (NODE_TYPE(IPHASE_NEXT(arg_node)) != NT_IPHASE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(iphase) has disallowed type(%s) ", nodetypeToName(IPHASE_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKiactions(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (IACTIONS_REFERENCE(arg_node)) {
        if (NODE_TYPE(IACTIONS_REFERENCE(arg_node)) != NT_ID) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(reference) of node(iactions) has disallowed type(%s) ", nodetypeToName(IACTIONS_REFERENCE(arg_node)));
        }

    }

    if (IACTIONS_NEXT(arg_node)) {
        if (NODE_TYPE(IACTIONS_NEXT(arg_node)) != NT_IACTIONS) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(next) of node(iactions) has disallowed type(%s) ", nodetypeToName(IACTIONS_NEXT(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

struct ccn_node *CHKast(struct ccn_node *arg_node) {
    size_t action_id = CCNgetCurrentActionId();
    (void)action_id;
    if (AST_IPHASES(arg_node)) {
        if (NODE_TYPE(AST_IPHASES(arg_node)) != NT_IPHASE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(iphases) of node(ast) has disallowed type(%s) ", nodetypeToName(AST_IPHASES(arg_node)));
        }

    }

    if (AST_ITRAVERSALS(arg_node)) {
        if (NODE_TYPE(AST_ITRAVERSALS(arg_node)) != NT_ITRAVERSAL) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(itraversals) of node(ast) has disallowed type(%s) ", nodetypeToName(AST_ITRAVERSALS(arg_node)));
        }

    }

    if (AST_IPASSES(arg_node)) {
        if (NODE_TYPE(AST_IPASSES(arg_node)) != NT_IPASS) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(ipasses) of node(ast) has disallowed type(%s) ", nodetypeToName(AST_IPASSES(arg_node)));
        }

    }

    if (AST_INODES(arg_node)) {
        if (NODE_TYPE(AST_INODES(arg_node)) != NT_INODE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(inodes) of node(ast) has disallowed type(%s) ", nodetypeToName(AST_INODES(arg_node)));
        }

    }

    if (AST_INODESETS(arg_node)) {
        if (NODE_TYPE(AST_INODESETS(arg_node)) != NT_INODESET) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(inodesets) of node(ast) has disallowed type(%s) ", nodetypeToName(AST_INODESETS(arg_node)));
        }

    }

    if (AST_ENUMS(arg_node)) {
        if (NODE_TYPE(AST_ENUMS(arg_node)) != NT_IENUM) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(enums) of node(ast) has disallowed type(%s) ", nodetypeToName(AST_ENUMS(arg_node)));
        }

    }

    if (AST_STABLE(arg_node)) {
        if (NODE_TYPE(AST_STABLE(arg_node)) != NT_STE) {
            CTI(CTI_ERROR, true, "Inconsistent node found in AST. Child(stable) of node(ast) has disallowed type(%s) ", nodetypeToName(AST_STABLE(arg_node)));
        }

    }

    TRAVchildren(arg_node);
    return arg_node;
}

