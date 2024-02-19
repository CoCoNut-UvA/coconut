#include "ccngen/ast.h"
#include <stddef.h>
#include "palm/str.h"
#include "palm/ctinfo.h"
#include "ccn/dynamic_core.h"

#ifdef __GNUC__
#define MAYBE_UNUSED __attribute__((unused))
#else
#define MAYBE_UNUSED
#endif

static const char *const user_warn MAYBE_UNUSED =
    "%s:%d: Attributes with user types do not support deep copying, "
    "instead the attributes are copied by value. Make sure you set "
    "a correct value for the copied node's attribute yourself. Add "
    "`#define CCN_USES_UNSAFE_ACKNOWLEDGE` to user_types.h to "
    "disable this warning.";

void CopyBaseNode(node_st *target, node_st *source) {
    NODE_BCOL(target) = NODE_BCOL(source);
    NODE_ECOL(target) = NODE_ECOL(source);
    NODE_BLINE(target) = NODE_BLINE(source);
    NODE_ELINE(target) = NODE_ELINE(source);
    NODE_FILENAME(target) = STRcpy(NODE_FILENAME(source));
}

ccn_node *CPYid(ccn_node *arg_node) {
    ccn_node *new_node = ASTid(NULL, NULL, NULL);
    CopyBaseNode(new_node, arg_node);
    ID_NEXT(new_node) = TRAVopt(ID_NEXT(arg_node));
    ID_ORIG(new_node) = STRcpy(ID_ORIG(arg_node));
    ID_LWR(new_node) = STRcpy(ID_LWR(arg_node));
    ID_UPR(new_node) = STRcpy(ID_UPR(arg_node));
    ID_ROW(new_node) = ID_ROW(arg_node);
    ID_COL_BEGIN(new_node) = ID_COL_BEGIN(arg_node);
    ID_COL_END(new_node) = ID_COL_END(arg_node);
    return new_node;
}

ccn_node *CPYienum(ccn_node *arg_node) {
    ccn_node *new_node = ASTienum(NULL, NULL, NULL, NULL);
    CopyBaseNode(new_node, arg_node);
    IENUM_VALS(new_node) = TRAVopt(IENUM_VALS(arg_node));
    IENUM_NAME(new_node) = TRAVopt(IENUM_NAME(arg_node));
    IENUM_IPREFIX(new_node) = TRAVopt(IENUM_IPREFIX(arg_node));
    IENUM_NEXT(new_node) = TRAVopt(IENUM_NEXT(arg_node));
    IENUM_IINFO(new_node) = STRcpy(IENUM_IINFO(arg_node));
    return new_node;
}

ccn_node *CPYvisit_sequence_dummy(ccn_node *arg_node) {
    ccn_node *new_node = ASTvisit_sequence_dummy(NULL);
    CopyBaseNode(new_node, arg_node);
    VISIT_SEQUENCE_DUMMY_ALT(new_node) = TRAVopt(VISIT_SEQUENCE_DUMMY_ALT(arg_node));
    VISIT_SEQUENCE_DUMMY_INODE(new_node) = VISIT_SEQUENCE_DUMMY_INODE(arg_node);
    return new_node;
}

ccn_node *CPYvisit_sequence_visit(ccn_node *arg_node) {
    ccn_node *new_node = ASTvisit_sequence_visit(NULL, NULL);
    CopyBaseNode(new_node, arg_node);
    VISIT_SEQUENCE_VISIT_ALT(new_node) = TRAVopt(VISIT_SEQUENCE_VISIT_ALT(arg_node));
    VISIT_SEQUENCE_VISIT_NEXT(new_node) = TRAVopt(VISIT_SEQUENCE_VISIT_NEXT(arg_node));
    VISIT_SEQUENCE_VISIT_CHILD(new_node) = VISIT_SEQUENCE_VISIT_CHILD(arg_node);
    VISIT_SEQUENCE_VISIT_VISIT(new_node) = VISIT_SEQUENCE_VISIT_VISIT(arg_node);
    return new_node;
}

ccn_node *CPYvisit_sequence_eval(ccn_node *arg_node) {
    ccn_node *new_node = ASTvisit_sequence_eval(NULL);
    CopyBaseNode(new_node, arg_node);
    VISIT_SEQUENCE_EVAL_NEXT(new_node) = TRAVopt(VISIT_SEQUENCE_EVAL_NEXT(arg_node));
    VISIT_SEQUENCE_EVAL_ATTRIBUTE(new_node) = VISIT_SEQUENCE_EVAL_ATTRIBUTE(arg_node);
    return new_node;
}

ccn_node *CPYvisit(ccn_node *arg_node) {
    ccn_node *new_node = ASTvisit(NULL, NULL, NULL, NULL, 0);
    CopyBaseNode(new_node, arg_node);
    VISIT_SEQUENCE(new_node) = TRAVopt(VISIT_SEQUENCE(arg_node));
    VISIT_INPUTS(new_node) = TRAVopt(VISIT_INPUTS(arg_node));
    VISIT_OUTPUTS(new_node) = TRAVopt(VISIT_OUTPUTS(arg_node));
    VISIT_NEXT(new_node) = TRAVopt(VISIT_NEXT(arg_node));
    VISIT_INODE(new_node) = VISIT_INODE(arg_node);
    VISIT_INDEX(new_node) = VISIT_INDEX(arg_node);
    return new_node;
}

ccn_node *CPYvisit_arg_list(ccn_node *arg_node) {
    ccn_node *new_node = ASTvisit_arg_list(NULL);
    CopyBaseNode(new_node, arg_node);
    VISIT_ARG_LIST_ATTRIBUTE(new_node) = TRAVopt(VISIT_ARG_LIST_ATTRIBUTE(arg_node));
    VISIT_ARG_LIST_NEXT(new_node) = TRAVopt(VISIT_ARG_LIST_NEXT(arg_node));
    return new_node;
}

ccn_node *CPYattribute_reference(ccn_node *arg_node) {
    ccn_node *new_node = ASTattribute_reference();
    CopyBaseNode(new_node, arg_node);
    ATTRIBUTE_REFERENCE_INODE(new_node) = TRAVopt(ATTRIBUTE_REFERENCE_INODE(arg_node));
    ATTRIBUTE_REFERENCE_IATTRIBUTE(new_node) = TRAVopt(ATTRIBUTE_REFERENCE_IATTRIBUTE(arg_node));
    ATTRIBUTE_REFERENCE_NODE_TYPE(new_node) = ATTRIBUTE_REFERENCE_NODE_TYPE(arg_node);
    ATTRIBUTE_REFERENCE_REFERENCE(new_node) = ATTRIBUTE_REFERENCE_REFERENCE(arg_node);
    ATTRIBUTE_REFERENCE_CHILD_REFERENCE(new_node) = ATTRIBUTE_REFERENCE_CHILD_REFERENCE(arg_node);
    return new_node;
}

ccn_node *CPYequation_dependency(ccn_node *arg_node) {
    ccn_node *new_node = ASTequation_dependency();
    CopyBaseNode(new_node, arg_node);
    EQUATION_DEPENDENCY_IATTRIBUTE(new_node) = TRAVopt(EQUATION_DEPENDENCY_IATTRIBUTE(arg_node));
    EQUATION_DEPENDENCY_NEXT(new_node) = TRAVopt(EQUATION_DEPENDENCY_NEXT(arg_node));
    return new_node;
}

ccn_node *CPYequation(ccn_node *arg_node) {
    ccn_node *new_node = ASTequation();
    CopyBaseNode(new_node, arg_node);
    EQUATION_RULE(new_node) = TRAVopt(EQUATION_RULE(arg_node));
    EQUATION_IARGS(new_node) = TRAVopt(EQUATION_IARGS(arg_node));
    EQUATION_NEXT(new_node) = TRAVopt(EQUATION_NEXT(arg_node));
    return new_node;
}

ccn_node *CPYattribute(ccn_node *arg_node) {
    ccn_node *new_node = ASTattribute();
    CopyBaseNode(new_node, arg_node);
    ATTRIBUTE_NAME(new_node) = TRAVopt(ATTRIBUTE_NAME(arg_node));
    ATTRIBUTE_TYPE_REFERENCE(new_node) = TRAVopt(ATTRIBUTE_TYPE_REFERENCE(arg_node));
    ATTRIBUTE_LIFETIMES(new_node) = TRAVopt(ATTRIBUTE_LIFETIMES(arg_node));
    ATTRIBUTE_NEXT(new_node) = TRAVopt(ATTRIBUTE_NEXT(arg_node));
    ATTRIBUTE_TYPE(new_node) = ATTRIBUTE_TYPE(arg_node);
    ATTRIBUTE_IN_CONSTRUCTOR(new_node) = ATTRIBUTE_IN_CONSTRUCTOR(arg_node);
    ATTRIBUTE_IS_INHERITED(new_node) = ATTRIBUTE_IS_INHERITED(arg_node);
    ATTRIBUTE_IS_SYNTHESIZED(new_node) = ATTRIBUTE_IS_SYNTHESIZED(arg_node);
    ATTRIBUTE_IS_PROPAGATED(new_node) = ATTRIBUTE_IS_PROPAGATED(arg_node);
    return new_node;
}

ccn_node *CPYitravdata(ccn_node *arg_node) {
    ccn_node *new_node = ASTitravdata(NULL);
    CopyBaseNode(new_node, arg_node);
    ITRAVDATA_NAME(new_node) = TRAVopt(ITRAVDATA_NAME(arg_node));
    ITRAVDATA_TYPE_REFERENCE(new_node) = TRAVopt(ITRAVDATA_TYPE_REFERENCE(arg_node));
    ITRAVDATA_NEXT(new_node) = TRAVopt(ITRAVDATA_NEXT(arg_node));
    ITRAVDATA_TYPE(new_node) = ITRAVDATA_TYPE(arg_node);
    ITRAVDATA_INCLUDE_FILE(new_node) = STRcpy(ITRAVDATA_INCLUDE_FILE(arg_node));
    return new_node;
}

ccn_node *CPYsetoperation(ccn_node *arg_node) {
    ccn_node *new_node = ASTsetoperation(NULL, NULL, 0);
    CopyBaseNode(new_node, arg_node);
    SETOPERATION_LEFT(new_node) = TRAVopt(SETOPERATION_LEFT(arg_node));
    SETOPERATION_RIGHT(new_node) = TRAVopt(SETOPERATION_RIGHT(arg_node));
    SETOPERATION_TYPE(new_node) = SETOPERATION_TYPE(arg_node);
    return new_node;
}

ccn_node *CPYsetliteral(ccn_node *arg_node) {
    ccn_node *new_node = ASTsetliteral(NULL);
    CopyBaseNode(new_node, arg_node);
    SETLITERAL_REFERENCE(new_node) = TRAVopt(SETLITERAL_REFERENCE(arg_node));
    SETLITERAL_LEFT(new_node) = TRAVopt(SETLITERAL_LEFT(arg_node));
    SETLITERAL_RIGHT(new_node) = TRAVopt(SETLITERAL_RIGHT(arg_node));
    return new_node;
}

ccn_node *CPYsetreference(ccn_node *arg_node) {
    ccn_node *new_node = ASTsetreference();
    CopyBaseNode(new_node, arg_node);
    SETREFERENCE_REFERENCE(new_node) = TRAVopt(SETREFERENCE_REFERENCE(arg_node));
    return new_node;
}

ccn_node *CPYste(ccn_node *arg_node) {
    ccn_node *new_node = ASTste();
    CopyBaseNode(new_node, arg_node);
    STE_NEXT(new_node) = TRAVopt(STE_NEXT(arg_node));
    STE_KEY(new_node) = STE_KEY(arg_node);
    STE_VALUE(new_node) = STE_VALUE(arg_node);
    return new_node;
}

ccn_node *CPYchild(ccn_node *arg_node) {
    ccn_node *new_node = ASTchild(NULL);
    CopyBaseNode(new_node, arg_node);
    CHILD_NAME(new_node) = TRAVopt(CHILD_NAME(arg_node));
    CHILD_LIFETIMES(new_node) = TRAVopt(CHILD_LIFETIMES(arg_node));
    CHILD_NEXT(new_node) = TRAVopt(CHILD_NEXT(arg_node));
    CHILD_TYPE_REFERENCE(new_node) = TRAVopt(CHILD_TYPE_REFERENCE(arg_node));
    CHILD_TYPE(new_node) = CHILD_TYPE(arg_node);
    CHILD_IN_CONSTRUCTOR(new_node) = CHILD_IN_CONSTRUCTOR(arg_node);
    CHILD_IS_MANDATORY(new_node) = CHILD_IS_MANDATORY(arg_node);
    return new_node;
}

ccn_node *CPYlifetime_range(ccn_node *arg_node) {
    ccn_node *new_node = ASTlifetime_range();
    CopyBaseNode(new_node, arg_node);
    LIFETIME_RANGE_TARGET(new_node) = TRAVopt(LIFETIME_RANGE_TARGET(arg_node));
    LIFETIME_RANGE_INCLUSIVE(new_node) = LIFETIME_RANGE_INCLUSIVE(arg_node);
    LIFETIME_RANGE_ACTION_ID(new_node) = LIFETIME_RANGE_ACTION_ID(arg_node);
    LIFETIME_RANGE_NEXT_ACTION_ID(new_node) = LIFETIME_RANGE_NEXT_ACTION_ID(arg_node);
    return new_node;
}

ccn_node *CPYilifetime(ccn_node *arg_node) {
    ccn_node *new_node = ASTilifetime();
    CopyBaseNode(new_node, arg_node);
    ILIFETIME_BEGIN(new_node) = TRAVopt(ILIFETIME_BEGIN(arg_node));
    ILIFETIME_END(new_node) = TRAVopt(ILIFETIME_END(arg_node));
    ILIFETIME_NEXT(new_node) = TRAVopt(ILIFETIME_NEXT(arg_node));
    ILIFETIME_TYPE(new_node) = ILIFETIME_TYPE(arg_node);
    return new_node;
}

ccn_node *CPYnodeset_child_entry(ccn_node *arg_node) {
    ccn_node *new_node = ASTnodeset_child_entry(NULL);
    CopyBaseNode(new_node, arg_node);
    NODESET_CHILD_ENTRY_NEXT(new_node) = TRAVopt(NODESET_CHILD_ENTRY_NEXT(arg_node));
    NODESET_CHILD_ENTRY_REFERENCE(new_node) = NODESET_CHILD_ENTRY_REFERENCE(arg_node);
    return new_node;
}

ccn_node *CPYinodeset(ccn_node *arg_node) {
    ccn_node *new_node = ASTinodeset();
    CopyBaseNode(new_node, arg_node);
    INODESET_NAME(new_node) = TRAVopt(INODESET_NAME(arg_node));
    INODESET_EXPR(new_node) = TRAVopt(INODESET_EXPR(arg_node));
    INODESET_IATTRIBUTES(new_node) = TRAVopt(INODESET_IATTRIBUTES(arg_node));
    INODESET_UNPACKED(new_node) = TRAVopt(INODESET_UNPACKED(arg_node));
    INODESET_NEXT(new_node) = TRAVopt(INODESET_NEXT(arg_node));
    INODESET_CHILDREN_TABLE(new_node) = TRAVopt(INODESET_CHILDREN_TABLE(arg_node));
    INODESET_IINFO(new_node) = STRcpy(INODESET_IINFO(arg_node));
    INODESET_ILLEGAL_SETEXPR_ATTR(new_node) = INODESET_ILLEGAL_SETEXPR_ATTR(arg_node);
    return new_node;
}

ccn_node *CPYinode(ccn_node *arg_node) {
    ccn_node *new_node = ASTinode(NULL, NULL);
    CopyBaseNode(new_node, arg_node);
    INODE_NAME(new_node) = TRAVopt(INODE_NAME(arg_node));
    INODE_NEXT(new_node) = TRAVopt(INODE_NEXT(arg_node));
    INODE_ICHILDREN(new_node) = TRAVopt(INODE_ICHILDREN(arg_node));
    INODE_IATTRIBUTES(new_node) = TRAVopt(INODE_IATTRIBUTES(arg_node));
    INODE_IEQUATIONS(new_node) = TRAVopt(INODE_IEQUATIONS(arg_node));
    INODE_LIFETIMES(new_node) = TRAVopt(INODE_LIFETIMES(arg_node));
    INODE_VISIT(new_node) = TRAVopt(INODE_VISIT(arg_node));
    INODE_IIFNO(new_node) = STRcpy(INODE_IIFNO(arg_node));
    INODE_IS_ROOT(new_node) = INODE_IS_ROOT(arg_node);
    INODE_INDEX(new_node) = INODE_INDEX(arg_node);
    return new_node;
}

ccn_node *CPYipass(ccn_node *arg_node) {
    ccn_node *new_node = ASTipass(NULL, NULL);
    CopyBaseNode(new_node, arg_node);
    IPASS_NAME(new_node) = TRAVopt(IPASS_NAME(arg_node));
    IPASS_IPREFIX(new_node) = TRAVopt(IPASS_IPREFIX(arg_node));
    IPASS_TARGET_FUNC(new_node) = TRAVopt(IPASS_TARGET_FUNC(arg_node));
    IPASS_NEXT(new_node) = TRAVopt(IPASS_NEXT(arg_node));
    IPASS_IIFNO(new_node) = STRcpy(IPASS_IIFNO(arg_node));
    return new_node;
}

ccn_node *CPYitraversal(ccn_node *arg_node) {
    ccn_node *new_node = ASTitraversal(NULL);
    CopyBaseNode(new_node, arg_node);
    ITRAVERSAL_NAME(new_node) = TRAVopt(ITRAVERSAL_NAME(arg_node));
    ITRAVERSAL_IPREFIX(new_node) = TRAVopt(ITRAVERSAL_IPREFIX(arg_node));
    ITRAVERSAL_INODES(new_node) = TRAVopt(ITRAVERSAL_INODES(arg_node));
    ITRAVERSAL_DATA(new_node) = TRAVopt(ITRAVERSAL_DATA(arg_node));
    ITRAVERSAL_NEXT(new_node) = TRAVopt(ITRAVERSAL_NEXT(arg_node));
    ITRAVERSAL_INDEX(new_node) = ITRAVERSAL_INDEX(arg_node);
    ITRAVERSAL_IINFO(new_node) = STRcpy(ITRAVERSAL_IINFO(arg_node));
    return new_node;
}

ccn_node *CPYiphase(ccn_node *arg_node) {
    ccn_node *new_node = ASTiphase(NULL, false);
    CopyBaseNode(new_node, arg_node);
    IPHASE_NAME(new_node) = TRAVopt(IPHASE_NAME(arg_node));
    IPHASE_IPREFIX(new_node) = TRAVopt(IPHASE_IPREFIX(arg_node));
    IPHASE_GATE_FUNC(new_node) = TRAVopt(IPHASE_GATE_FUNC(arg_node));
    IPHASE_IACTIONS(new_node) = TRAVopt(IPHASE_IACTIONS(arg_node));
    IPHASE_NEXT(new_node) = TRAVopt(IPHASE_NEXT(arg_node));
    IPHASE_IS_START(new_node) = IPHASE_IS_START(arg_node);
    IPHASE_IS_CYCLE(new_node) = IPHASE_IS_CYCLE(arg_node);
    IPHASE_IINFO(new_node) = STRcpy(IPHASE_IINFO(arg_node));
    return new_node;
}

ccn_node *CPYiactions(ccn_node *arg_node) {
    ccn_node *new_node = ASTiactions();
    CopyBaseNode(new_node, arg_node);
    IACTIONS_REFERENCE(new_node) = TRAVopt(IACTIONS_REFERENCE(arg_node));
    IACTIONS_NEXT(new_node) = TRAVopt(IACTIONS_NEXT(arg_node));
    IACTIONS_ACTION_ID(new_node) = IACTIONS_ACTION_ID(arg_node);
    return new_node;
}

ccn_node *CPYast(ccn_node *arg_node) {
    ccn_node *new_node = ASTast();
    CopyBaseNode(new_node, arg_node);
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
    AST_USES_UNSAFE(new_node) = AST_USES_UNSAFE(arg_node);
    return new_node;
}

