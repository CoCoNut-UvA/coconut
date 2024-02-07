#include "ccngen/ast.h"
#include "palm/memory.h"
node_st *NewNode() {
    node_st *node = MEMmalloc(sizeof(node_st));
    NODE_TYPE(node) = NT_NULL;
    NODE_CHILDREN(node) = NULL;
    NODE_FILENAME(node) = NULL;
    NODE_NUMCHILDREN(node) = 0;
    NODE_BLINE(node) = 0;
    NODE_ELINE(node) = 0;
    NODE_BCOL(node) = 0;
    NODE_ECOL(node) = 0;
    return node;
}

node_st *ASTid(char * orig, char * lwr, char * Upr) {
    node_st *node = NewNode();
    node->data.N_id = MEMmalloc(sizeof(struct NODE_DATA_ID));
    NODE_TYPE(node) = NT_ID;
    ID_NEXT(node) = NULL;
    ID_ORIG(node) = orig;
    ID_LWR(node) = lwr;
    ID_UPR(node) = Upr;
    ID_ROW(node) = 0;
    ID_COL_BEGIN(node) = 0;
    ID_COL_END(node) = 0;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_id->id_children.id_children_at;
    return node;}

node_st *ASTienum(node_st *vals, node_st *name, node_st *iprefix, char * iinfo) {
    node_st *node = NewNode();
    node->data.N_ienum = MEMmalloc(sizeof(struct NODE_DATA_IENUM));
    NODE_TYPE(node) = NT_IENUM;
    IENUM_VALS(node) = vals;
    IENUM_NAME(node) = name;
    IENUM_IPREFIX(node) = iprefix;
    IENUM_NEXT(node) = NULL;
    IENUM_IINFO(node) = iinfo;
    NODE_NUMCHILDREN(node) = 4;
    NODE_CHILDREN(node) = node->data.N_ienum->ienum_children.ienum_children_at;
    return node;}

node_st *ASTvisit_sequence_dummy(node_st *inode) {
    node_st *node = NewNode();
    node->data.N_visit_sequence_dummy = MEMmalloc(sizeof(struct NODE_DATA_VISIT_SEQUENCE_DUMMY));
    NODE_TYPE(node) = NT_VISIT_SEQUENCE_DUMMY;
    VISIT_SEQUENCE_DUMMY_ALT(node) = NULL;
    VISIT_SEQUENCE_DUMMY_INODE(node) = inode;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_visit_sequence_dummy->visit_sequence_dummy_children.visit_sequence_dummy_children_at;
    return node;}

node_st *ASTvisit_sequence_visit(node_st *child, node_st *visit) {
    node_st *node = NewNode();
    node->data.N_visit_sequence_visit = MEMmalloc(sizeof(struct NODE_DATA_VISIT_SEQUENCE_VISIT));
    NODE_TYPE(node) = NT_VISIT_SEQUENCE_VISIT;
    VISIT_SEQUENCE_VISIT_ALT(node) = NULL;
    VISIT_SEQUENCE_VISIT_NEXT(node) = NULL;
    VISIT_SEQUENCE_VISIT_CHILD(node) = child;
    VISIT_SEQUENCE_VISIT_VISIT(node) = visit;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_visit_sequence_visit->visit_sequence_visit_children.visit_sequence_visit_children_at;
    return node;}

node_st *ASTvisit_sequence_eval(node_st *attribute) {
    node_st *node = NewNode();
    node->data.N_visit_sequence_eval = MEMmalloc(sizeof(struct NODE_DATA_VISIT_SEQUENCE_EVAL));
    NODE_TYPE(node) = NT_VISIT_SEQUENCE_EVAL;
    VISIT_SEQUENCE_EVAL_NEXT(node) = NULL;
    VISIT_SEQUENCE_EVAL_ATTRIBUTE(node) = attribute;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_visit_sequence_eval->visit_sequence_eval_children.visit_sequence_eval_children_at;
    return node;}

node_st *ASTvisit(node_st *sequence, node_st *inputs, node_st *outputs, node_st *inode, uint64_t index) {
    node_st *node = NewNode();
    node->data.N_visit = MEMmalloc(sizeof(struct NODE_DATA_VISIT));
    NODE_TYPE(node) = NT_VISIT;
    VISIT_SEQUENCE(node) = sequence;
    VISIT_INPUTS(node) = inputs;
    VISIT_OUTPUTS(node) = outputs;
    VISIT_NEXT(node) = NULL;
    VISIT_INODE(node) = inode;
    VISIT_INDEX(node) = index;
    NODE_NUMCHILDREN(node) = 4;
    NODE_CHILDREN(node) = node->data.N_visit->visit_children.visit_children_at;
    return node;}

node_st *ASTvisit_arg_list(node_st *attribute) {
    node_st *node = NewNode();
    node->data.N_visit_arg_list = MEMmalloc(sizeof(struct NODE_DATA_VISIT_ARG_LIST));
    NODE_TYPE(node) = NT_VISIT_ARG_LIST;
    VISIT_ARG_LIST_ATTRIBUTE(node) = attribute;
    VISIT_ARG_LIST_NEXT(node) = NULL;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_visit_arg_list->visit_arg_list_children.visit_arg_list_children_at;
    return node;}

node_st *ASTattribute_reference(void) {
    node_st *node = NewNode();
    node->data.N_attribute_reference = MEMmalloc(sizeof(struct NODE_DATA_ATTRIBUTE_REFERENCE));
    NODE_TYPE(node) = NT_ATTRIBUTE_REFERENCE;
    ATTRIBUTE_REFERENCE_INODE(node) = NULL;
    ATTRIBUTE_REFERENCE_IATTRIBUTE(node) = NULL;
    ATTRIBUTE_REFERENCE_NODE_TYPE(node) = NULL;
    ATTRIBUTE_REFERENCE_REFERENCE(node) = NULL;
    ATTRIBUTE_REFERENCE_CHILD_REFERENCE(node) = NULL;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_attribute_reference->attribute_reference_children.attribute_reference_children_at;
    return node;}

node_st *ASTequation_dependency(void) {
    node_st *node = NewNode();
    node->data.N_equation_dependency = MEMmalloc(sizeof(struct NODE_DATA_EQUATION_DEPENDENCY));
    NODE_TYPE(node) = NT_EQUATION_DEPENDENCY;
    EQUATION_DEPENDENCY_IATTRIBUTE(node) = NULL;
    EQUATION_DEPENDENCY_NEXT(node) = NULL;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_equation_dependency->equation_dependency_children.equation_dependency_children_at;
    return node;}

node_st *ASTequation(void) {
    node_st *node = NewNode();
    node->data.N_equation = MEMmalloc(sizeof(struct NODE_DATA_EQUATION));
    NODE_TYPE(node) = NT_EQUATION;
    EQUATION_RULE(node) = NULL;
    EQUATION_IARGS(node) = NULL;
    EQUATION_NEXT(node) = NULL;
    NODE_NUMCHILDREN(node) = 3;
    NODE_CHILDREN(node) = node->data.N_equation->equation_children.equation_children_at;
    return node;}

node_st *ASTattribute(void) {
    node_st *node = NewNode();
    node->data.N_attribute = MEMmalloc(sizeof(struct NODE_DATA_ATTRIBUTE));
    NODE_TYPE(node) = NT_ATTRIBUTE;
    ATTRIBUTE_NAME(node) = NULL;
    ATTRIBUTE_TYPE_REFERENCE(node) = NULL;
    ATTRIBUTE_LIFETIMES(node) = NULL;
    ATTRIBUTE_NEXT(node) = NULL;
    ATTRIBUTE_TYPE(node) = 0;
    ATTRIBUTE_IN_CONSTRUCTOR(node) = false;
    ATTRIBUTE_IS_INHERITED(node) = false;
    ATTRIBUTE_IS_SYNTHESIZED(node) = false;
    ATTRIBUTE_IS_PROPAGATED(node) = false;
    NODE_NUMCHILDREN(node) = 4;
    NODE_CHILDREN(node) = node->data.N_attribute->attribute_children.attribute_children_at;
    return node;}

node_st *ASTitravdata(node_st *name) {
    node_st *node = NewNode();
    node->data.N_itravdata = MEMmalloc(sizeof(struct NODE_DATA_ITRAVDATA));
    NODE_TYPE(node) = NT_ITRAVDATA;
    ITRAVDATA_NAME(node) = name;
    ITRAVDATA_TYPE_REFERENCE(node) = NULL;
    ITRAVDATA_NEXT(node) = NULL;
    ITRAVDATA_TYPE(node) = 0;
    ITRAVDATA_INCLUDE_FILE(node) = NULL;
    NODE_NUMCHILDREN(node) = 3;
    NODE_CHILDREN(node) = node->data.N_itravdata->itravdata_children.itravdata_children_at;
    return node;}

node_st *ASTsetoperation(node_st *left, node_st *right, enum setoperation_type type) {
    node_st *node = NewNode();
    node->data.N_setoperation = MEMmalloc(sizeof(struct NODE_DATA_SETOPERATION));
    NODE_TYPE(node) = NT_SETOPERATION;
    SETOPERATION_LEFT(node) = left;
    SETOPERATION_RIGHT(node) = right;
    SETOPERATION_TYPE(node) = type;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_setoperation->setoperation_children.setoperation_children_at;
    return node;}

node_st *ASTsetliteral(node_st *reference) {
    node_st *node = NewNode();
    node->data.N_setliteral = MEMmalloc(sizeof(struct NODE_DATA_SETLITERAL));
    NODE_TYPE(node) = NT_SETLITERAL;
    SETLITERAL_REFERENCE(node) = reference;
    SETLITERAL_LEFT(node) = NULL;
    SETLITERAL_RIGHT(node) = NULL;
    NODE_NUMCHILDREN(node) = 3;
    NODE_CHILDREN(node) = node->data.N_setliteral->setliteral_children.setliteral_children_at;
    return node;}

node_st *ASTsetreference(void) {
    node_st *node = NewNode();
    node->data.N_setreference = MEMmalloc(sizeof(struct NODE_DATA_SETREFERENCE));
    NODE_TYPE(node) = NT_SETREFERENCE;
    SETREFERENCE_REFERENCE(node) = NULL;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_setreference->setreference_children.setreference_children_at;
    return node;}

node_st *ASTste(void) {
    node_st *node = NewNode();
    node->data.N_ste = MEMmalloc(sizeof(struct NODE_DATA_STE));
    NODE_TYPE(node) = NT_STE;
    STE_NEXT(node) = NULL;
    STE_KEY(node) = NULL;
    STE_VALUE(node) = NULL;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_ste->ste_children.ste_children_at;
    return node;}

node_st *ASTchild(node_st *name) {
    node_st *node = NewNode();
    node->data.N_child = MEMmalloc(sizeof(struct NODE_DATA_CHILD));
    NODE_TYPE(node) = NT_CHILD;
    CHILD_NAME(node) = name;
    CHILD_LIFETIMES(node) = NULL;
    CHILD_NEXT(node) = NULL;
    CHILD_TYPE_REFERENCE(node) = NULL;
    CHILD_TYPE(node) = 0;
    CHILD_IN_CONSTRUCTOR(node) = false;
    CHILD_IS_MANDATORY(node) = false;
    NODE_NUMCHILDREN(node) = 4;
    NODE_CHILDREN(node) = node->data.N_child->child_children.child_children_at;
    return node;}

node_st *ASTlifetime_range(void) {
    node_st *node = NewNode();
    node->data.N_lifetime_range = MEMmalloc(sizeof(struct NODE_DATA_LIFETIME_RANGE));
    NODE_TYPE(node) = NT_LIFETIME_RANGE;
    LIFETIME_RANGE_TARGET(node) = NULL;
    LIFETIME_RANGE_INCLUSIVE(node) = false;
    LIFETIME_RANGE_ACTION_ID(node) = 0;
    LIFETIME_RANGE_NEXT_ACTION_ID(node) = 0;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_lifetime_range->lifetime_range_children.lifetime_range_children_at;
    return node;}

node_st *ASTilifetime(void) {
    node_st *node = NewNode();
    node->data.N_ilifetime = MEMmalloc(sizeof(struct NODE_DATA_ILIFETIME));
    NODE_TYPE(node) = NT_ILIFETIME;
    ILIFETIME_BEGIN(node) = NULL;
    ILIFETIME_END(node) = NULL;
    ILIFETIME_NEXT(node) = NULL;
    ILIFETIME_TYPE(node) = 0;
    NODE_NUMCHILDREN(node) = 3;
    NODE_CHILDREN(node) = node->data.N_ilifetime->ilifetime_children.ilifetime_children_at;
    return node;}

node_st *ASTnodeset_child_entry(node_st *reference) {
    node_st *node = NewNode();
    node->data.N_nodeset_child_entry = MEMmalloc(sizeof(struct NODE_DATA_NODESET_CHILD_ENTRY));
    NODE_TYPE(node) = NT_NODESET_CHILD_ENTRY;
    NODESET_CHILD_ENTRY_NEXT(node) = NULL;
    NODESET_CHILD_ENTRY_REFERENCE(node) = reference;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_nodeset_child_entry->nodeset_child_entry_children.nodeset_child_entry_children_at;
    return node;}

node_st *ASTinodeset(void) {
    node_st *node = NewNode();
    node->data.N_inodeset = MEMmalloc(sizeof(struct NODE_DATA_INODESET));
    NODE_TYPE(node) = NT_INODESET;
    INODESET_NAME(node) = NULL;
    INODESET_EXPR(node) = NULL;
    INODESET_IATTRIBUTES(node) = NULL;
    INODESET_UNPACKED(node) = NULL;
    INODESET_NEXT(node) = NULL;
    INODESET_CHILDREN_TABLE(node) = NULL;
    INODESET_IINFO(node) = NULL;
    INODESET_ILLEGAL_SETEXPR_ATTR(node) = false;
    NODE_NUMCHILDREN(node) = 6;
    NODE_CHILDREN(node) = node->data.N_inodeset->inodeset_children.inodeset_children_at;
    return node;}

node_st *ASTinode(node_st *name, char * iifno) {
    node_st *node = NewNode();
    node->data.N_inode = MEMmalloc(sizeof(struct NODE_DATA_INODE));
    NODE_TYPE(node) = NT_INODE;
    INODE_NAME(node) = name;
    INODE_NEXT(node) = NULL;
    INODE_ICHILDREN(node) = NULL;
    INODE_IATTRIBUTES(node) = NULL;
    INODE_IEQUATIONS(node) = NULL;
    INODE_LIFETIMES(node) = NULL;
    INODE_VISIT(node) = NULL;
    INODE_IIFNO(node) = iifno;
    INODE_IS_ROOT(node) = false;
    INODE_INDEX(node) = 0;
    NODE_NUMCHILDREN(node) = 7;
    NODE_CHILDREN(node) = node->data.N_inode->inode_children.inode_children_at;
    return node;}

node_st *ASTipass(node_st *name, char * iifno) {
    node_st *node = NewNode();
    node->data.N_ipass = MEMmalloc(sizeof(struct NODE_DATA_IPASS));
    NODE_TYPE(node) = NT_IPASS;
    IPASS_NAME(node) = name;
    IPASS_IPREFIX(node) = NULL;
    IPASS_TARGET_FUNC(node) = NULL;
    IPASS_NEXT(node) = NULL;
    IPASS_IIFNO(node) = iifno;
    NODE_NUMCHILDREN(node) = 4;
    NODE_CHILDREN(node) = node->data.N_ipass->ipass_children.ipass_children_at;
    return node;}

node_st *ASTitraversal(node_st *name) {
    node_st *node = NewNode();
    node->data.N_itraversal = MEMmalloc(sizeof(struct NODE_DATA_ITRAVERSAL));
    NODE_TYPE(node) = NT_ITRAVERSAL;
    ITRAVERSAL_NAME(node) = name;
    ITRAVERSAL_IPREFIX(node) = NULL;
    ITRAVERSAL_INODES(node) = NULL;
    ITRAVERSAL_DATA(node) = NULL;
    ITRAVERSAL_NEXT(node) = NULL;
    ITRAVERSAL_INDEX(node) = 0;
    ITRAVERSAL_IINFO(node) = NULL;
    NODE_NUMCHILDREN(node) = 5;
    NODE_CHILDREN(node) = node->data.N_itraversal->itraversal_children.itraversal_children_at;
    return node;}

node_st *ASTiphase(node_st *name, bool is_start) {
    node_st *node = NewNode();
    node->data.N_iphase = MEMmalloc(sizeof(struct NODE_DATA_IPHASE));
    NODE_TYPE(node) = NT_IPHASE;
    IPHASE_NAME(node) = name;
    IPHASE_IPREFIX(node) = NULL;
    IPHASE_GATE_FUNC(node) = NULL;
    IPHASE_IACTIONS(node) = NULL;
    IPHASE_NEXT(node) = NULL;
    IPHASE_IS_START(node) = is_start;
    IPHASE_IS_CYCLE(node) = false;
    IPHASE_IINFO(node) = NULL;
    NODE_NUMCHILDREN(node) = 5;
    NODE_CHILDREN(node) = node->data.N_iphase->iphase_children.iphase_children_at;
    return node;}

node_st *ASTiactions(void) {
    node_st *node = NewNode();
    node->data.N_iactions = MEMmalloc(sizeof(struct NODE_DATA_IACTIONS));
    NODE_TYPE(node) = NT_IACTIONS;
    IACTIONS_REFERENCE(node) = NULL;
    IACTIONS_NEXT(node) = NULL;
    IACTIONS_ACTION_ID(node) = 0;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_iactions->iactions_children.iactions_children_at;
    return node;}

node_st *ASTast(void) {
    node_st *node = NewNode();
    node->data.N_ast = MEMmalloc(sizeof(struct NODE_DATA_AST));
    NODE_TYPE(node) = NT_AST;
    AST_IPHASES(node) = NULL;
    AST_ITRAVERSALS(node) = NULL;
    AST_IPASSES(node) = NULL;
    AST_INODES(node) = NULL;
    AST_INODESETS(node) = NULL;
    AST_ENUMS(node) = NULL;
    AST_STABLE(node) = NULL;
    AST_NUM_TRAVERSALS(node) = 0;
    AST_NUM_NODES(node) = 0;
    AST_ROOT_NODE(node) = NULL;
    AST_START_PHASE(node) = NULL;
    AST_USES_UNSAFE(node) = false;
    NODE_NUMCHILDREN(node) = 7;
    NODE_CHILDREN(node) = node->data.N_ast->ast_children.ast_children_at;
    return node;}

