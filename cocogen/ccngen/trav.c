#include <stdbool.h>
#include "palm/dbug.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
node_st *TRAVlifetimes(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_INODE:
            INODE_LIFETIMES(node) = TRAVopt(INODE_LIFETIMES(node));
            break;
            break;
        case NT_CHILD:
            CHILD_LIFETIMES(node) = TRAVopt(CHILD_LIFETIMES(node));
            break;
            break;
        case NT_ATTRIBUTE:
            ATTRIBUTE_LIFETIMES(node) = TRAVopt(ATTRIBUTE_LIFETIMES(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child lifetimes");
            break;
    }

    return node;
}

node_st *TRAVinodes(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_AST:
            AST_INODES(node) = TRAVopt(AST_INODES(node));
            break;
            break;
        case NT_ITRAVERSAL:
            ITRAVERSAL_INODES(node) = TRAVopt(ITRAVERSAL_INODES(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child inodes");
            break;
    }

    return node;
}

node_st *TRAVdata(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_ITRAVERSAL:
            ITRAVERSAL_DATA(node) = TRAVopt(ITRAVERSAL_DATA(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child data");
            break;
    }

    return node;
}

node_st *TRAViactions(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_IPHASE:
            IPHASE_IACTIONS(node) = TRAVopt(IPHASE_IACTIONS(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child iactions");
            break;
    }

    return node;
}

node_st *TRAVipasses(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_AST:
            AST_IPASSES(node) = TRAVopt(AST_IPASSES(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child ipasses");
            break;
    }

    return node;
}

node_st *TRAVinodesets(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_AST:
            AST_INODESETS(node) = TRAVopt(AST_INODESETS(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child inodesets");
            break;
    }

    return node;
}

node_st *TRAVreference(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_IACTIONS:
            IACTIONS_REFERENCE(node) = TRAVopt(IACTIONS_REFERENCE(node));
            break;
            break;
        case NT_SETREFERENCE:
            SETREFERENCE_REFERENCE(node) = TRAVopt(SETREFERENCE_REFERENCE(node));
            break;
            break;
        case NT_SETLITERAL:
            SETLITERAL_REFERENCE(node) = TRAVopt(SETLITERAL_REFERENCE(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child reference");
            break;
    }

    return node;
}

node_st *TRAVtarget(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_LIFETIME_RANGE:
            LIFETIME_RANGE_TARGET(node) = TRAVopt(LIFETIME_RANGE_TARGET(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child target");
            break;
    }

    return node;
}

node_st *TRAVbegin(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_ILIFETIME:
            ILIFETIME_BEGIN(node) = TRAVopt(ILIFETIME_BEGIN(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child begin");
            break;
    }

    return node;
}

node_st *TRAVstable(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_AST:
            AST_STABLE(node) = TRAVopt(AST_STABLE(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child stable");
            break;
    }

    return node;
}

node_st *TRAVgate_func(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_IPHASE:
            IPHASE_GATE_FUNC(node) = TRAVopt(IPHASE_GATE_FUNC(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child gate_func");
            break;
    }

    return node;
}

node_st *TRAVend(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_ILIFETIME:
            ILIFETIME_END(node) = TRAVopt(ILIFETIME_END(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child end");
            break;
    }

    return node;
}

node_st *TRAVunpacked(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_INODESET:
            INODESET_UNPACKED(node) = TRAVopt(INODESET_UNPACKED(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child unpacked");
            break;
    }

    return node;
}

node_st *TRAVichildren(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_INODE:
            INODE_ICHILDREN(node) = TRAVopt(INODE_ICHILDREN(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child ichildren");
            break;
    }

    return node;
}

node_st *TRAVVals(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_IENUM:
            IENUM_VALS(node) = TRAVopt(IENUM_VALS(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child Vals");
            break;
    }

    return node;
}

node_st *TRAVnext(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_IACTIONS:
            IACTIONS_NEXT(node) = TRAVopt(IACTIONS_NEXT(node));
            break;
            break;
        case NT_IPHASE:
            IPHASE_NEXT(node) = TRAVopt(IPHASE_NEXT(node));
            break;
            break;
        case NT_ITRAVERSAL:
            ITRAVERSAL_NEXT(node) = TRAVopt(ITRAVERSAL_NEXT(node));
            break;
            break;
        case NT_IPASS:
            IPASS_NEXT(node) = TRAVopt(IPASS_NEXT(node));
            break;
            break;
        case NT_INODE:
            INODE_NEXT(node) = TRAVopt(INODE_NEXT(node));
            break;
            break;
        case NT_INODESET:
            INODESET_NEXT(node) = TRAVopt(INODESET_NEXT(node));
            break;
            break;
        case NT_ILIFETIME:
            ILIFETIME_NEXT(node) = TRAVopt(ILIFETIME_NEXT(node));
            break;
            break;
        case NT_CHILD:
            CHILD_NEXT(node) = TRAVopt(CHILD_NEXT(node));
            break;
            break;
        case NT_STE:
            STE_NEXT(node) = TRAVopt(STE_NEXT(node));
            break;
            break;
        case NT_ITRAVDATA:
            ITRAVDATA_NEXT(node) = TRAVopt(ITRAVDATA_NEXT(node));
            break;
            break;
        case NT_ATTRIBUTE:
            ATTRIBUTE_NEXT(node) = TRAVopt(ATTRIBUTE_NEXT(node));
            break;
            break;
        case NT_IENUM:
            IENUM_NEXT(node) = TRAVopt(IENUM_NEXT(node));
            break;
            break;
        case NT_ID:
            ID_NEXT(node) = TRAVopt(ID_NEXT(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child next");
            break;
    }

    return node;
}

node_st *TRAVname(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_IPHASE:
            IPHASE_NAME(node) = TRAVopt(IPHASE_NAME(node));
            break;
            break;
        case NT_ITRAVERSAL:
            ITRAVERSAL_NAME(node) = TRAVopt(ITRAVERSAL_NAME(node));
            break;
            break;
        case NT_IPASS:
            IPASS_NAME(node) = TRAVopt(IPASS_NAME(node));
            break;
            break;
        case NT_INODE:
            INODE_NAME(node) = TRAVopt(INODE_NAME(node));
            break;
            break;
        case NT_INODESET:
            INODESET_NAME(node) = TRAVopt(INODESET_NAME(node));
            break;
            break;
        case NT_CHILD:
            CHILD_NAME(node) = TRAVopt(CHILD_NAME(node));
            break;
            break;
        case NT_ITRAVDATA:
            ITRAVDATA_NAME(node) = TRAVopt(ITRAVDATA_NAME(node));
            break;
            break;
        case NT_ATTRIBUTE:
            ATTRIBUTE_NAME(node) = TRAVopt(ATTRIBUTE_NAME(node));
            break;
            break;
        case NT_IENUM:
            IENUM_NAME(node) = TRAVopt(IENUM_NAME(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child name");
            break;
    }

    return node;
}

node_st *TRAVleft(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_SETLITERAL:
            SETLITERAL_LEFT(node) = TRAVopt(SETLITERAL_LEFT(node));
            break;
            break;
        case NT_SETOPERATION:
            SETOPERATION_LEFT(node) = TRAVopt(SETOPERATION_LEFT(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child left");
            break;
    }

    return node;
}

node_st *TRAVtype_reference(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_CHILD:
            CHILD_TYPE_REFERENCE(node) = TRAVopt(CHILD_TYPE_REFERENCE(node));
            break;
            break;
        case NT_ITRAVDATA:
            ITRAVDATA_TYPE_REFERENCE(node) = TRAVopt(ITRAVDATA_TYPE_REFERENCE(node));
            break;
            break;
        case NT_ATTRIBUTE:
            ATTRIBUTE_TYPE_REFERENCE(node) = TRAVopt(ATTRIBUTE_TYPE_REFERENCE(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child type_reference");
            break;
    }

    return node;
}

node_st *TRAViphases(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_AST:
            AST_IPHASES(node) = TRAVopt(AST_IPHASES(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child iphases");
            break;
    }

    return node;
}

node_st *TRAVright(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_SETLITERAL:
            SETLITERAL_RIGHT(node) = TRAVopt(SETLITERAL_RIGHT(node));
            break;
            break;
        case NT_SETOPERATION:
            SETOPERATION_RIGHT(node) = TRAVopt(SETOPERATION_RIGHT(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child right");
            break;
    }

    return node;
}

node_st *TRAViattributes(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_INODE:
            INODE_IATTRIBUTES(node) = TRAVopt(INODE_IATTRIBUTES(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child iattributes");
            break;
    }

    return node;
}

node_st *TRAViprefix(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_IPHASE:
            IPHASE_IPREFIX(node) = TRAVopt(IPHASE_IPREFIX(node));
            break;
            break;
        case NT_ITRAVERSAL:
            ITRAVERSAL_IPREFIX(node) = TRAVopt(ITRAVERSAL_IPREFIX(node));
            break;
            break;
        case NT_IPASS:
            IPASS_IPREFIX(node) = TRAVopt(IPASS_IPREFIX(node));
            break;
            break;
        case NT_IENUM:
            IENUM_IPREFIX(node) = TRAVopt(IENUM_IPREFIX(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child iprefix");
            break;
    }

    return node;
}

node_st *TRAVexpr(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_INODESET:
            INODESET_EXPR(node) = TRAVopt(INODESET_EXPR(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child expr");
            break;
    }

    return node;
}

node_st *TRAVtarget_func(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_IPASS:
            IPASS_TARGET_FUNC(node) = TRAVopt(IPASS_TARGET_FUNC(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child target_func");
            break;
    }

    return node;
}

node_st *TRAVenums(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_AST:
            AST_ENUMS(node) = TRAVopt(AST_ENUMS(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child enums");
            break;
    }

    return node;
}

node_st *TRAVitraversals(node_st *node) {
    switch (NODE_TYPE(node)) {
        case NT_AST:
            AST_ITRAVERSALS(node) = TRAVopt(AST_ITRAVERSALS(node));
            break;
            break;
        default:
            DBUG_ASSERT(false, "Current target has no child itraversals");
            break;
    }

    return node;
}

