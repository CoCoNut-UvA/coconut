#include "ccngen/ast.h"
#include "palm/memory.h"
#include "palm/dbug.h"
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
    return node;
}

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
    return node;
}

node_st *ASTvisit_sequence_dummy(node_st *inode) {
    node_st *node = NewNode();
    node->data.N_visit_sequence_dummy = MEMmalloc(sizeof(struct NODE_DATA_VISIT_SEQUENCE_DUMMY));
    NODE_TYPE(node) = NT_VISIT_SEQUENCE_DUMMY;
    VISIT_SEQUENCE_DUMMY_ALT(node) = NULL;
    VISIT_SEQUENCE_DUMMY_INODE(node) = inode;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_visit_sequence_dummy->visit_sequence_dummy_children.visit_sequence_dummy_children_at;
    return node;
}

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
    return node;
}

node_st *ASTvisit_sequence_eval(node_st *attribute) {
    node_st *node = NewNode();
    node->data.N_visit_sequence_eval = MEMmalloc(sizeof(struct NODE_DATA_VISIT_SEQUENCE_EVAL));
    NODE_TYPE(node) = NT_VISIT_SEQUENCE_EVAL;
    VISIT_SEQUENCE_EVAL_NEXT(node) = NULL;
    VISIT_SEQUENCE_EVAL_ATTRIBUTE(node) = attribute;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_visit_sequence_eval->visit_sequence_eval_children.visit_sequence_eval_children_at;
    return node;
}

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
    return node;
}

node_st *ASTvisit_arg_list(node_st *attribute) {
    node_st *node = NewNode();
    node->data.N_visit_arg_list = MEMmalloc(sizeof(struct NODE_DATA_VISIT_ARG_LIST));
    NODE_TYPE(node) = NT_VISIT_ARG_LIST;
    VISIT_ARG_LIST_ATTRIBUTE(node) = attribute;
    VISIT_ARG_LIST_NEXT(node) = NULL;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_visit_arg_list->visit_arg_list_children.visit_arg_list_children_at;
    return node;
}

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
    return node;
}

node_st *ASTequation_dependency(void) {
    node_st *node = NewNode();
    node->data.N_equation_dependency = MEMmalloc(sizeof(struct NODE_DATA_EQUATION_DEPENDENCY));
    NODE_TYPE(node) = NT_EQUATION_DEPENDENCY;
    EQUATION_DEPENDENCY_IATTRIBUTE(node) = NULL;
    EQUATION_DEPENDENCY_NEXT(node) = NULL;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_equation_dependency->equation_dependency_children.equation_dependency_children_at;
    return node;
}

node_st *ASTequation(void) {
    node_st *node = NewNode();
    node->data.N_equation = MEMmalloc(sizeof(struct NODE_DATA_EQUATION));
    NODE_TYPE(node) = NT_EQUATION;
    EQUATION_RULE(node) = NULL;
    EQUATION_IARGS(node) = NULL;
    EQUATION_NEXT(node) = NULL;
    NODE_NUMCHILDREN(node) = 3;
    NODE_CHILDREN(node) = node->data.N_equation->equation_children.equation_children_at;
    return node;
}

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
    return node;
}

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
    return node;
}

node_st *ASTsetoperation(node_st *left, node_st *right, enum setoperation_type type) {
    node_st *node = NewNode();
    node->data.N_setoperation = MEMmalloc(sizeof(struct NODE_DATA_SETOPERATION));
    NODE_TYPE(node) = NT_SETOPERATION;
    SETOPERATION_LEFT(node) = left;
    SETOPERATION_RIGHT(node) = right;
    SETOPERATION_TYPE(node) = type;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_setoperation->setoperation_children.setoperation_children_at;
    return node;
}

node_st *ASTsetliteral(node_st *reference) {
    node_st *node = NewNode();
    node->data.N_setliteral = MEMmalloc(sizeof(struct NODE_DATA_SETLITERAL));
    NODE_TYPE(node) = NT_SETLITERAL;
    SETLITERAL_REFERENCE(node) = reference;
    SETLITERAL_LEFT(node) = NULL;
    SETLITERAL_RIGHT(node) = NULL;
    NODE_NUMCHILDREN(node) = 3;
    NODE_CHILDREN(node) = node->data.N_setliteral->setliteral_children.setliteral_children_at;
    return node;
}

node_st *ASTsetreference(void) {
    node_st *node = NewNode();
    node->data.N_setreference = MEMmalloc(sizeof(struct NODE_DATA_SETREFERENCE));
    NODE_TYPE(node) = NT_SETREFERENCE;
    SETREFERENCE_REFERENCE(node) = NULL;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_setreference->setreference_children.setreference_children_at;
    return node;
}

node_st *ASTste(void) {
    node_st *node = NewNode();
    node->data.N_ste = MEMmalloc(sizeof(struct NODE_DATA_STE));
    NODE_TYPE(node) = NT_STE;
    STE_NEXT(node) = NULL;
    STE_KEY(node) = NULL;
    STE_VALUE(node) = NULL;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_ste->ste_children.ste_children_at;
    return node;
}

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
    return node;
}

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
    return node;
}

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
    return node;
}

node_st *ASTnodeset_child_entry(node_st *reference) {
    node_st *node = NewNode();
    node->data.N_nodeset_child_entry = MEMmalloc(sizeof(struct NODE_DATA_NODESET_CHILD_ENTRY));
    NODE_TYPE(node) = NT_NODESET_CHILD_ENTRY;
    NODESET_CHILD_ENTRY_NEXT(node) = NULL;
    NODESET_CHILD_ENTRY_REFERENCE(node) = reference;
    NODE_NUMCHILDREN(node) = 1;
    NODE_CHILDREN(node) = node->data.N_nodeset_child_entry->nodeset_child_entry_children.nodeset_child_entry_children_at;
    return node;
}

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
    return node;
}

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
    return node;
}

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
    return node;
}

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
    return node;
}

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
    return node;
}

node_st *ASTiactions(void) {
    node_st *node = NewNode();
    node->data.N_iactions = MEMmalloc(sizeof(struct NODE_DATA_IACTIONS));
    NODE_TYPE(node) = NT_IACTIONS;
    IACTIONS_REFERENCE(node) = NULL;
    IACTIONS_NEXT(node) = NULL;
    IACTIONS_ACTION_ID(node) = 0;
    NODE_NUMCHILDREN(node) = 2;
    NODE_CHILDREN(node) = node->data.N_iactions->iactions_children.iactions_children_at;
    return node;
}

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
    return node;
}

#ifdef NDEBUG
// Ignore unused parameters for debug info
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
// Ignore unused parameters
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#endif

void CCNaccesserror_id_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ID_NEXT is not a 'id'");
#endif
}

void CCNaccesserror_id_orig(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ID_ORIG is not a 'id'");
#endif
}

void CCNaccesserror_id_lwr(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ID_LWR is not a 'id'");
#endif
}

void CCNaccesserror_id_upr(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ID_UPR is not a 'id'");
#endif
}

void CCNaccesserror_id_row(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ID_ROW is not a 'id'");
#endif
}

void CCNaccesserror_id_col_begin(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ID_COL_BEGIN is not a 'id'");
#endif
}

void CCNaccesserror_id_col_end(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ID_COL_END is not a 'id'");
#endif
}

void CCNaccesserror_ienum_vals(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IENUM_VALS is not a 'ienum'");
#endif
}

void CCNaccesserror_ienum_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IENUM_NAME is not a 'ienum'");
#endif
}

void CCNaccesserror_ienum_iprefix(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IENUM_IPREFIX is not a 'ienum'");
#endif
}

void CCNaccesserror_ienum_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IENUM_NEXT is not a 'ienum'");
#endif
}

void CCNaccesserror_ienum_iinfo(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IENUM_IINFO is not a 'ienum'");
#endif
}

void CCNaccesserror_visit_sequence_dummy_alt(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE_DUMMY_ALT is not a 'visit_sequence_dummy'");
#endif
}

void CCNaccesserror_visit_sequence_dummy_inode(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE_DUMMY_INODE is not a 'visit_sequence_dummy'");
#endif
}

void CCNaccesserror_visit_sequence_visit_alt(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE_VISIT_ALT is not a 'visit_sequence_visit'");
#endif
}

void CCNaccesserror_visit_sequence_visit_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE_VISIT_NEXT is not a 'visit_sequence_visit'");
#endif
}

void CCNaccesserror_visit_sequence_visit_child(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE_VISIT_CHILD is not a 'visit_sequence_visit'");
#endif
}

void CCNaccesserror_visit_sequence_visit_visit(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE_VISIT_VISIT is not a 'visit_sequence_visit'");
#endif
}

void CCNaccesserror_visit_sequence_eval_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE_EVAL_NEXT is not a 'visit_sequence_eval'");
#endif
}

void CCNaccesserror_visit_sequence_eval_attribute(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE_EVAL_ATTRIBUTE is not a 'visit_sequence_eval'");
#endif
}

void CCNaccesserror_visit_sequence(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_SEQUENCE is not a 'visit'");
#endif
}

void CCNaccesserror_visit_inputs(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_INPUTS is not a 'visit'");
#endif
}

void CCNaccesserror_visit_outputs(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_OUTPUTS is not a 'visit'");
#endif
}

void CCNaccesserror_visit_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_NEXT is not a 'visit'");
#endif
}

void CCNaccesserror_visit_inode(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_INODE is not a 'visit'");
#endif
}

void CCNaccesserror_visit_index(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_INDEX is not a 'visit'");
#endif
}

void CCNaccesserror_visit_arg_list_attribute(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_ARG_LIST_ATTRIBUTE is not a 'visit_arg_list'");
#endif
}

void CCNaccesserror_visit_arg_list_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to VISIT_ARG_LIST_NEXT is not a 'visit_arg_list'");
#endif
}

void CCNaccesserror_attribute_reference_inode(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_REFERENCE_INODE is not a 'attribute_reference'");
#endif
}

void CCNaccesserror_attribute_reference_iattribute(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_REFERENCE_IATTRIBUTE is not a 'attribute_reference'");
#endif
}

void CCNaccesserror_attribute_reference_node_type(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_REFERENCE_NODE_TYPE is not a 'attribute_reference'");
#endif
}

void CCNaccesserror_attribute_reference_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_REFERENCE_REFERENCE is not a 'attribute_reference'");
#endif
}

void CCNaccesserror_attribute_reference_child_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_REFERENCE_CHILD_REFERENCE is not a 'attribute_reference'");
#endif
}

void CCNaccesserror_equation_dependency_iattribute(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to EQUATION_DEPENDENCY_IATTRIBUTE is not a 'equation_dependency'");
#endif
}

void CCNaccesserror_equation_dependency_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to EQUATION_DEPENDENCY_NEXT is not a 'equation_dependency'");
#endif
}

void CCNaccesserror_equation_rule(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to EQUATION_RULE is not a 'equation'");
#endif
}

void CCNaccesserror_equation_iargs(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to EQUATION_IARGS is not a 'equation'");
#endif
}

void CCNaccesserror_equation_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to EQUATION_NEXT is not a 'equation'");
#endif
}

void CCNaccesserror_attribute_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_NAME is not a 'attribute'");
#endif
}

void CCNaccesserror_attribute_type_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_TYPE_REFERENCE is not a 'attribute'");
#endif
}

void CCNaccesserror_attribute_lifetimes(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_LIFETIMES is not a 'attribute'");
#endif
}

void CCNaccesserror_attribute_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_NEXT is not a 'attribute'");
#endif
}

void CCNaccesserror_attribute_type(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_TYPE is not a 'attribute'");
#endif
}

void CCNaccesserror_attribute_in_constructor(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_IN_CONSTRUCTOR is not a 'attribute'");
#endif
}

void CCNaccesserror_attribute_is_inherited(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_IS_INHERITED is not a 'attribute'");
#endif
}

void CCNaccesserror_attribute_is_synthesized(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_IS_SYNTHESIZED is not a 'attribute'");
#endif
}

void CCNaccesserror_attribute_is_propagated(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ATTRIBUTE_IS_PROPAGATED is not a 'attribute'");
#endif
}

void CCNaccesserror_itravdata_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVDATA_NAME is not a 'itravdata'");
#endif
}

void CCNaccesserror_itravdata_type_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVDATA_TYPE_REFERENCE is not a 'itravdata'");
#endif
}

void CCNaccesserror_itravdata_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVDATA_NEXT is not a 'itravdata'");
#endif
}

void CCNaccesserror_itravdata_type(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVDATA_TYPE is not a 'itravdata'");
#endif
}

void CCNaccesserror_itravdata_include_file(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVDATA_INCLUDE_FILE is not a 'itravdata'");
#endif
}

void CCNaccesserror_setoperation_left(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to SETOPERATION_LEFT is not a 'setoperation'");
#endif
}

void CCNaccesserror_setoperation_right(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to SETOPERATION_RIGHT is not a 'setoperation'");
#endif
}

void CCNaccesserror_setoperation_type(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to SETOPERATION_TYPE is not a 'setoperation'");
#endif
}

void CCNaccesserror_setliteral_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to SETLITERAL_REFERENCE is not a 'setliteral'");
#endif
}

void CCNaccesserror_setliteral_left(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to SETLITERAL_LEFT is not a 'setliteral'");
#endif
}

void CCNaccesserror_setliteral_right(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to SETLITERAL_RIGHT is not a 'setliteral'");
#endif
}

void CCNaccesserror_setreference_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to SETREFERENCE_REFERENCE is not a 'setreference'");
#endif
}

void CCNaccesserror_ste_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to STE_NEXT is not a 'ste'");
#endif
}

void CCNaccesserror_ste_key(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to STE_KEY is not a 'ste'");
#endif
}

void CCNaccesserror_ste_value(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to STE_VALUE is not a 'ste'");
#endif
}

void CCNaccesserror_child_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to CHILD_NAME is not a 'child'");
#endif
}

void CCNaccesserror_child_lifetimes(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to CHILD_LIFETIMES is not a 'child'");
#endif
}

void CCNaccesserror_child_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to CHILD_NEXT is not a 'child'");
#endif
}

void CCNaccesserror_child_type_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to CHILD_TYPE_REFERENCE is not a 'child'");
#endif
}

void CCNaccesserror_child_type(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to CHILD_TYPE is not a 'child'");
#endif
}

void CCNaccesserror_child_in_constructor(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to CHILD_IN_CONSTRUCTOR is not a 'child'");
#endif
}

void CCNaccesserror_child_is_mandatory(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to CHILD_IS_MANDATORY is not a 'child'");
#endif
}

void CCNaccesserror_lifetime_range_target(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to LIFETIME_RANGE_TARGET is not a 'lifetime_range'");
#endif
}

void CCNaccesserror_lifetime_range_inclusive(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to LIFETIME_RANGE_INCLUSIVE is not a 'lifetime_range'");
#endif
}

void CCNaccesserror_lifetime_range_action_id(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to LIFETIME_RANGE_ACTION_ID is not a 'lifetime_range'");
#endif
}

void CCNaccesserror_lifetime_range_next_action_id(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to LIFETIME_RANGE_NEXT_ACTION_ID is not a 'lifetime_range'");
#endif
}

void CCNaccesserror_ilifetime_begin(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ILIFETIME_BEGIN is not a 'ilifetime'");
#endif
}

void CCNaccesserror_ilifetime_end(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ILIFETIME_END is not a 'ilifetime'");
#endif
}

void CCNaccesserror_ilifetime_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ILIFETIME_NEXT is not a 'ilifetime'");
#endif
}

void CCNaccesserror_ilifetime_type(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ILIFETIME_TYPE is not a 'ilifetime'");
#endif
}

void CCNaccesserror_nodeset_child_entry_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to NODESET_CHILD_ENTRY_NEXT is not a 'nodeset_child_entry'");
#endif
}

void CCNaccesserror_nodeset_child_entry_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to NODESET_CHILD_ENTRY_REFERENCE is not a 'nodeset_child_entry'");
#endif
}

void CCNaccesserror_inodeset_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODESET_NAME is not a 'inodeset'");
#endif
}

void CCNaccesserror_inodeset_expr(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODESET_EXPR is not a 'inodeset'");
#endif
}

void CCNaccesserror_inodeset_iattributes(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODESET_IATTRIBUTES is not a 'inodeset'");
#endif
}

void CCNaccesserror_inodeset_unpacked(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODESET_UNPACKED is not a 'inodeset'");
#endif
}

void CCNaccesserror_inodeset_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODESET_NEXT is not a 'inodeset'");
#endif
}

void CCNaccesserror_inodeset_children_table(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODESET_CHILDREN_TABLE is not a 'inodeset'");
#endif
}

void CCNaccesserror_inodeset_iinfo(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODESET_IINFO is not a 'inodeset'");
#endif
}

void CCNaccesserror_inodeset_illegal_setexpr_attr(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODESET_ILLEGAL_SETEXPR_ATTR is not a 'inodeset'");
#endif
}

void CCNaccesserror_inode_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_NAME is not a 'inode'");
#endif
}

void CCNaccesserror_inode_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_NEXT is not a 'inode'");
#endif
}

void CCNaccesserror_inode_ichildren(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_ICHILDREN is not a 'inode'");
#endif
}

void CCNaccesserror_inode_iattributes(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_IATTRIBUTES is not a 'inode'");
#endif
}

void CCNaccesserror_inode_iequations(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_IEQUATIONS is not a 'inode'");
#endif
}

void CCNaccesserror_inode_lifetimes(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_LIFETIMES is not a 'inode'");
#endif
}

void CCNaccesserror_inode_visit(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_VISIT is not a 'inode'");
#endif
}

void CCNaccesserror_inode_iifno(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_IIFNO is not a 'inode'");
#endif
}

void CCNaccesserror_inode_is_root(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_IS_ROOT is not a 'inode'");
#endif
}

void CCNaccesserror_inode_index(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to INODE_INDEX is not a 'inode'");
#endif
}

void CCNaccesserror_ipass_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPASS_NAME is not a 'ipass'");
#endif
}

void CCNaccesserror_ipass_iprefix(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPASS_IPREFIX is not a 'ipass'");
#endif
}

void CCNaccesserror_ipass_target_func(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPASS_TARGET_FUNC is not a 'ipass'");
#endif
}

void CCNaccesserror_ipass_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPASS_NEXT is not a 'ipass'");
#endif
}

void CCNaccesserror_ipass_iifno(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPASS_IIFNO is not a 'ipass'");
#endif
}

void CCNaccesserror_itraversal_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVERSAL_NAME is not a 'itraversal'");
#endif
}

void CCNaccesserror_itraversal_iprefix(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVERSAL_IPREFIX is not a 'itraversal'");
#endif
}

void CCNaccesserror_itraversal_inodes(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVERSAL_INODES is not a 'itraversal'");
#endif
}

void CCNaccesserror_itraversal_data(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVERSAL_DATA is not a 'itraversal'");
#endif
}

void CCNaccesserror_itraversal_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVERSAL_NEXT is not a 'itraversal'");
#endif
}

void CCNaccesserror_itraversal_index(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVERSAL_INDEX is not a 'itraversal'");
#endif
}

void CCNaccesserror_itraversal_iinfo(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to ITRAVERSAL_IINFO is not a 'itraversal'");
#endif
}

void CCNaccesserror_iphase_name(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPHASE_NAME is not a 'iphase'");
#endif
}

void CCNaccesserror_iphase_iprefix(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPHASE_IPREFIX is not a 'iphase'");
#endif
}

void CCNaccesserror_iphase_gate_func(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPHASE_GATE_FUNC is not a 'iphase'");
#endif
}

void CCNaccesserror_iphase_iactions(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPHASE_IACTIONS is not a 'iphase'");
#endif
}

void CCNaccesserror_iphase_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPHASE_NEXT is not a 'iphase'");
#endif
}

void CCNaccesserror_iphase_is_start(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPHASE_IS_START is not a 'iphase'");
#endif
}

void CCNaccesserror_iphase_is_cycle(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPHASE_IS_CYCLE is not a 'iphase'");
#endif
}

void CCNaccesserror_iphase_iinfo(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IPHASE_IINFO is not a 'iphase'");
#endif
}

void CCNaccesserror_iactions_reference(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IACTIONS_REFERENCE is not a 'iactions'");
#endif
}

void CCNaccesserror_iactions_next(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IACTIONS_NEXT is not a 'iactions'");
#endif
}

void CCNaccesserror_iactions_action_id(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to IACTIONS_ACTION_ID is not a 'iactions'");
#endif
}

void CCNaccesserror_ast_iphases(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_IPHASES is not a 'ast'");
#endif
}

void CCNaccesserror_ast_itraversals(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_ITRAVERSALS is not a 'ast'");
#endif
}

void CCNaccesserror_ast_ipasses(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_IPASSES is not a 'ast'");
#endif
}

void CCNaccesserror_ast_inodes(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_INODES is not a 'ast'");
#endif
}

void CCNaccesserror_ast_inodesets(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_INODESETS is not a 'ast'");
#endif
}

void CCNaccesserror_ast_enums(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_ENUMS is not a 'ast'");
#endif
}

void CCNaccesserror_ast_stable(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_STABLE is not a 'ast'");
#endif
}

void CCNaccesserror_ast_num_traversals(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_NUM_TRAVERSALS is not a 'ast'");
#endif
}

void CCNaccesserror_ast_num_nodes(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_NUM_NODES is not a 'ast'");
#endif
}

void CCNaccesserror_ast_root_node(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_ROOT_NODE is not a 'ast'");
#endif
}

void CCNaccesserror_ast_start_phase(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_START_PHASE is not a 'ast'");
#endif
}

void CCNaccesserror_ast_uses_unsafe(int line, const char *file, const char *func) {
#ifndef NDEBUG
    DBUGprintAssert(line, (char *)file, func, "Node passed to AST_USES_UNSAFE is not a 'ast'");
#endif
}

#ifdef NDEBUG
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#endif

