#include "ccngen/ast.h"
#include <stddef.h>
#include "palm/str.h"
#include "ccn/dynamic_core.h"
struct ccn_node *CPYid(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTid(    NULL    ,     NULL    ,     NULL);
    ID_NEXT(new_node) = TRAVopt(ID_NEXT(arg_node));
    ID_ORIG(new_node) = STRcpy(ID_ORIG(arg_node));
    ID_LWR(new_node) = STRcpy(ID_LWR(arg_node));
    ID_UPR(new_node) = STRcpy(ID_UPR(arg_node));
    ID_ROW(new_node) = ID_ROW(arg_node);
    ID_COL_BEGIN(new_node) = ID_COL_BEGIN(arg_node);
    ID_COL_END(new_node) = ID_COL_END(arg_node);
    return new_node;
}

struct ccn_node *CPYienum(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTienum(    NULL    ,     NULL    ,     NULL    ,     NULL);
    IENUM_VALS(new_node) = TRAVopt(IENUM_VALS(arg_node));
    IENUM_NAME(new_node) = TRAVopt(IENUM_NAME(arg_node));
    IENUM_IPREFIX(new_node) = TRAVopt(IENUM_IPREFIX(arg_node));
    IENUM_NEXT(new_node) = TRAVopt(IENUM_NEXT(arg_node));
    IENUM_IINFO(new_node) = STRcpy(IENUM_IINFO(arg_node));
    return new_node;
}

struct ccn_node *CPYattribute(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTattribute();
    ATTRIBUTE_NAME(new_node) = TRAVopt(ATTRIBUTE_NAME(arg_node));
    ATTRIBUTE_TYPE_REFERENCE(new_node) = TRAVopt(ATTRIBUTE_TYPE_REFERENCE(arg_node));
    ATTRIBUTE_NEXT(new_node) = TRAVopt(ATTRIBUTE_NEXT(arg_node));
    ATTRIBUTE_TYPE(new_node) = ATTRIBUTE_TYPE(arg_node);
    ATTRIBUTE_IN_CONSTRUCTOR(new_node) = ATTRIBUTE_IN_CONSTRUCTOR(arg_node);
    return new_node;
}

struct ccn_node *CPYitravdata(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTitravdata(    NULL);
    ITRAVDATA_NAME(new_node) = TRAVopt(ITRAVDATA_NAME(arg_node));
    ITRAVDATA_TYPE_REFERENCE(new_node) = TRAVopt(ITRAVDATA_TYPE_REFERENCE(arg_node));
    ITRAVDATA_NEXT(new_node) = TRAVopt(ITRAVDATA_NEXT(arg_node));
    ITRAVDATA_TYPE(new_node) = ITRAVDATA_TYPE(arg_node);
    ITRAVDATA_INCLUDE_FILE(new_node) = STRcpy(ITRAVDATA_INCLUDE_FILE(arg_node));
    return new_node;
}

struct ccn_node *CPYsetoperation(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTsetoperation(    NULL    ,     NULL    ,     0);
    SETOPERATION_LEFT(new_node) = TRAVopt(SETOPERATION_LEFT(arg_node));
    SETOPERATION_RIGHT(new_node) = TRAVopt(SETOPERATION_RIGHT(arg_node));
    SETOPERATION_TYPE(new_node) = SETOPERATION_TYPE(arg_node);
    return new_node;
}

struct ccn_node *CPYsetliteral(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTsetliteral();
    SETLITERAL_REFERENCE(new_node) = TRAVopt(SETLITERAL_REFERENCE(arg_node));
    SETLITERAL_NEXT(new_node) = TRAVopt(SETLITERAL_NEXT(arg_node));
    return new_node;
}

struct ccn_node *CPYsetreference(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTsetreference();
    SETREFERENCE_REFERENCE(new_node) = TRAVopt(SETREFERENCE_REFERENCE(arg_node));
    return new_node;
}

struct ccn_node *CPYste(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTste();
    STE_NEXT(new_node) = TRAVopt(STE_NEXT(arg_node));
    STE_KEY(new_node) = STE_KEY(arg_node);
    STE_VALUE(new_node) = STE_VALUE(arg_node);
    return new_node;
}

struct ccn_node *CPYchild(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTchild(    NULL);
    CHILD_NAME(new_node) = TRAVopt(CHILD_NAME(arg_node));
    CHILD_TYPE_REFERENCE(new_node) = TRAVopt(CHILD_TYPE_REFERENCE(arg_node));
    CHILD_NEXT(new_node) = TRAVopt(CHILD_NEXT(arg_node));
    CHILD_TYPE(new_node) = CHILD_TYPE(arg_node);
    CHILD_IN_CONSTRUCTOR(new_node) = CHILD_IN_CONSTRUCTOR(arg_node);
    CHILD_IS_MANDATORY(new_node) = CHILD_IS_MANDATORY(arg_node);
    return new_node;
}

struct ccn_node *CPYlifetime_range(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTlifetime_range();
    LIFETIME_RANGE_BEGIN(new_node) = TRAVopt(LIFETIME_RANGE_BEGIN(arg_node));
    LIFETIME_RANGE_END(new_node) = TRAVopt(LIFETIME_RANGE_END(arg_node));
    return new_node;
}

struct ccn_node *CPYilifetime(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTilifetime();
    ILIFETIME_BEGIN(new_node) = TRAVopt(ILIFETIME_BEGIN(arg_node));
    ILIFETIME_END(new_node) = TRAVopt(ILIFETIME_END(arg_node));
    ILIFETIME_NEXT(new_node) = TRAVopt(ILIFETIME_NEXT(arg_node));
    ILIFETIME_TYPE(new_node) = ILIFETIME_TYPE(arg_node);
    return new_node;
}

struct ccn_node *CPYinodeset(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTinodeset();
    INODESET_NAME(new_node) = TRAVopt(INODESET_NAME(arg_node));
    INODESET_EXPR(new_node) = TRAVopt(INODESET_EXPR(arg_node));
    INODESET_UNPACKED(new_node) = TRAVopt(INODESET_UNPACKED(arg_node));
    INODESET_NEXT(new_node) = TRAVopt(INODESET_NEXT(arg_node));
    INODESET_IINFO(new_node) = STRcpy(INODESET_IINFO(arg_node));
    return new_node;
}

struct ccn_node *CPYinode(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTinode(    NULL    ,     NULL);
    INODE_NAME(new_node) = TRAVopt(INODE_NAME(arg_node));
    INODE_NEXT(new_node) = TRAVopt(INODE_NEXT(arg_node));
    INODE_ICHILDREN(new_node) = TRAVopt(INODE_ICHILDREN(arg_node));
    INODE_IATTRIBUTES(new_node) = TRAVopt(INODE_IATTRIBUTES(arg_node));
    INODE_LIFETIMES(new_node) = TRAVopt(INODE_LIFETIMES(arg_node));
    INODE_IIFNO(new_node) = STRcpy(INODE_IIFNO(arg_node));
    INODE_IS_ROOT(new_node) = INODE_IS_ROOT(arg_node);
    INODE_INDEX(new_node) = INODE_INDEX(arg_node);
    return new_node;
}

struct ccn_node *CPYipass(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTipass(    NULL    ,     NULL    ,     NULL);
    IPASS_NEXT(new_node) = TRAVopt(IPASS_NEXT(arg_node));
    IPASS_NAME(new_node) = IPASS_NAME(arg_node);
    IPASS_IIFNO(new_node) = STRcpy(IPASS_IIFNO(arg_node));
    IPASS_IPREFIX(new_node) = IPASS_IPREFIX(arg_node);
    IPASS_TARGET_FUNC(new_node) = IPASS_TARGET_FUNC(arg_node);
    return new_node;
}

struct ccn_node *CPYitraversal(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTitraversal(    NULL);
    ITRAVERSAL_INODES(new_node) = TRAVopt(ITRAVERSAL_INODES(arg_node));
    ITRAVERSAL_DATA(new_node) = TRAVopt(ITRAVERSAL_DATA(arg_node));
    ITRAVERSAL_NEXT(new_node) = TRAVopt(ITRAVERSAL_NEXT(arg_node));
    ITRAVERSAL_INDEX(new_node) = ITRAVERSAL_INDEX(arg_node);
    ITRAVERSAL_NAME(new_node) = ITRAVERSAL_NAME(arg_node);
    ITRAVERSAL_IINFO(new_node) = STRcpy(ITRAVERSAL_IINFO(arg_node));
    ITRAVERSAL_IPREFIX(new_node) = ITRAVERSAL_IPREFIX(arg_node);
    return new_node;
}

struct ccn_node *CPYiphase(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTiphase(    NULL    ,     0);
    IPHASE_IACTIONS(new_node) = TRAVopt(IPHASE_IACTIONS(arg_node));
    IPHASE_NEXT(new_node) = TRAVopt(IPHASE_NEXT(arg_node));
    IPHASE_NAME(new_node) = IPHASE_NAME(arg_node);
    IPHASE_IS_START(new_node) = IPHASE_IS_START(arg_node);
    IPHASE_IS_CYCLE(new_node) = IPHASE_IS_CYCLE(arg_node);
    IPHASE_IINFO(new_node) = STRcpy(IPHASE_IINFO(arg_node));
    IPHASE_IPREFIX(new_node) = IPHASE_IPREFIX(arg_node);
    IPHASE_GATE_FUNC(new_node) = IPHASE_GATE_FUNC(arg_node);
    return new_node;
}

struct ccn_node *CPYiactions(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTiactions();
    IACTIONS_NEXT(new_node) = TRAVopt(IACTIONS_NEXT(arg_node));
    IACTIONS_REFERENCE(new_node) = IACTIONS_REFERENCE(arg_node);
    IACTIONS_ACTION_ID(new_node) = IACTIONS_ACTION_ID(arg_node);
    return new_node;
}

struct ccn_node *CPYast(struct ccn_node *arg_node) {
    struct ccn_node *new_node = ASTast();
    AST_IPHASES(new_node) = TRAVopt(AST_IPHASES(arg_node));
    AST_ITRAVERSALS(new_node) = TRAVopt(AST_ITRAVERSALS(arg_node));
    AST_IPASSES(new_node) = TRAVopt(AST_IPASSES(arg_node));
    AST_INODES(new_node) = TRAVopt(AST_INODES(arg_node));
    AST_INODESETS(new_node) = TRAVopt(AST_INODESETS(arg_node));
    AST_ENUMS(new_node) = TRAVopt(AST_ENUMS(arg_node));
    AST_STABLE(new_node) = TRAVopt(AST_STABLE(arg_node));
    AST_NUM_TRAVERSALS(new_node) = AST_NUM_TRAVERSALS(arg_node);
    AST_NUM_NODES(new_node) = AST_NUM_NODES(arg_node);
    AST_ROOT_NODE(new_node) = AST_ROOT_NODE(arg_node);
    AST_START_PHASE(new_node) = AST_START_PHASE(arg_node);
    return new_node;
}

