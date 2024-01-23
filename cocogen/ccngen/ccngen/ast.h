#pragma once
#include <assert.h>

#include "ccn/ccn_types.h"
#include "ccngen/enum.h"
#ifdef CCN_USES_UNSAFE
#include "user_types.h"
#endif
typedef struct ccn_node node_st;
struct NODE_DATA_ID {
    union NODE_CHILDREN_ID {
        struct NODE_CHILDREN_ID_STRUCT {
            node_st *next;
        } id_children_st;

        node_st *id_children_at[1];
    } id_children;

    char * orig;
    char * lwr;
    char * upr;
    int row;
    int col_begin;
    int col_end;
};

struct NODE_DATA_IENUM {
    union NODE_CHILDREN_IENUM {
        struct NODE_CHILDREN_IENUM_STRUCT {
            node_st *vals;
            node_st *name;
            node_st *iprefix;
            node_st *next;
        } ienum_children_st;

        node_st *ienum_children_at[4];
    } ienum_children;

    char * iinfo;
};

struct NODE_DATA_VISIT_SEQUENCE_VISIT {
    union NODE_CHILDREN_VISIT_SEQUENCE_VISIT {
        struct NODE_CHILDREN_VISIT_SEQUENCE_VISIT_STRUCT {
            node_st *alt;
            node_st *next;
        } visit_sequence_visit_children_st;

        node_st *visit_sequence_visit_children_at[2];
    } visit_sequence_visit_children;

    node_st *child;
    node_st *visit;
};

struct NODE_DATA_VISIT_SEQUENCE_EVAL {
    union NODE_CHILDREN_VISIT_SEQUENCE_EVAL {
        struct NODE_CHILDREN_VISIT_SEQUENCE_EVAL_STRUCT {
            node_st *next;
        } visit_sequence_eval_children_st;

        node_st *visit_sequence_eval_children_at[1];
    } visit_sequence_eval_children;

    node_st *attribute;
};

struct NODE_DATA_VISIT {
    union NODE_CHILDREN_VISIT {
        struct NODE_CHILDREN_VISIT_STRUCT {
            node_st *sequence;
            node_st *inputs;
            node_st *outputs;
            node_st *next;
        } visit_children_st;

        node_st *visit_children_at[4];
    } visit_children;

    node_st *inode;
    uint64_t index;
};

struct NODE_DATA_VISIT_ARG_LIST {
    union NODE_CHILDREN_VISIT_ARG_LIST {
        struct NODE_CHILDREN_VISIT_ARG_LIST_STRUCT {
            node_st *attribute;
            node_st *next;
        } visit_arg_list_children_st;

        node_st *visit_arg_list_children_at[2];
    } visit_arg_list_children;

};

struct NODE_DATA_ATTRIBUTE_REFERENCE {
    union NODE_CHILDREN_ATTRIBUTE_REFERENCE {
        struct NODE_CHILDREN_ATTRIBUTE_REFERENCE_STRUCT {
            node_st *inode;
            node_st *iattribute;
        } attribute_reference_children_st;

        node_st *attribute_reference_children_at[2];
    } attribute_reference_children;

    node_st *node_type;
    node_st *reference;
    node_st *child_reference;
};

struct NODE_DATA_EQUATION_DEPENDENCY {
    union NODE_CHILDREN_EQUATION_DEPENDENCY {
        struct NODE_CHILDREN_EQUATION_DEPENDENCY_STRUCT {
            node_st *iattribute;
            node_st *next;
        } equation_dependency_children_st;

        node_st *equation_dependency_children_at[2];
    } equation_dependency_children;

};

struct NODE_DATA_EQUATION {
    union NODE_CHILDREN_EQUATION {
        struct NODE_CHILDREN_EQUATION_STRUCT {
            node_st *rule;
            node_st *iargs;
            node_st *next;
        } equation_children_st;

        node_st *equation_children_at[3];
    } equation_children;

};

struct NODE_DATA_ATTRIBUTE {
    union NODE_CHILDREN_ATTRIBUTE {
        struct NODE_CHILDREN_ATTRIBUTE_STRUCT {
            node_st *name;
            node_st *type_reference;
            node_st *lifetimes;
            node_st *next;
        } attribute_children_st;

        node_st *attribute_children_at[4];
    } attribute_children;

    enum attribute_type type;
    bool in_constructor;
    bool is_inherited;
    bool is_synthesized;
    bool is_propagated;
};

struct NODE_DATA_ITRAVDATA {
    union NODE_CHILDREN_ITRAVDATA {
        struct NODE_CHILDREN_ITRAVDATA_STRUCT {
            node_st *name;
            node_st *type_reference;
            node_st *next;
        } itravdata_children_st;

        node_st *itravdata_children_at[3];
    } itravdata_children;

    enum attribute_type type;
    char * include_file;
};

struct NODE_DATA_SETOPERATION {
    union NODE_CHILDREN_SETOPERATION {
        struct NODE_CHILDREN_SETOPERATION_STRUCT {
            node_st *left;
            node_st *right;
        } setoperation_children_st;

        node_st *setoperation_children_at[2];
    } setoperation_children;

    enum setoperation_type type;
};

struct NODE_DATA_SETLITERAL {
    union NODE_CHILDREN_SETLITERAL {
        struct NODE_CHILDREN_SETLITERAL_STRUCT {
            node_st *reference;
            node_st *left;
            node_st *right;
        } setliteral_children_st;

        node_st *setliteral_children_at[3];
    } setliteral_children;

};

struct NODE_DATA_SETREFERENCE {
    union NODE_CHILDREN_SETREFERENCE {
        struct NODE_CHILDREN_SETREFERENCE_STRUCT {
            node_st *reference;
        } setreference_children_st;

        node_st *setreference_children_at[1];
    } setreference_children;

};

struct NODE_DATA_STE {
    union NODE_CHILDREN_STE {
        struct NODE_CHILDREN_STE_STRUCT {
            node_st *next;
        } ste_children_st;

        node_st *ste_children_at[1];
    } ste_children;

    node_st *key;
    node_st *value;
};

struct NODE_DATA_CHILD {
    union NODE_CHILDREN_CHILD {
        struct NODE_CHILDREN_CHILD_STRUCT {
            node_st *name;
            node_st *lifetimes;
            node_st *next;
            node_st *type_reference;
        } child_children_st;

        node_st *child_children_at[4];
    } child_children;

    enum child_type type;
    int in_constructor;
    int is_mandatory;
};

struct NODE_DATA_LIFETIME_RANGE {
    union NODE_CHILDREN_LIFETIME_RANGE {
        struct NODE_CHILDREN_LIFETIME_RANGE_STRUCT {
            node_st *target;
        } lifetime_range_children_st;

        node_st *lifetime_range_children_at[1];
    } lifetime_range_children;

    bool inclusive;
    int action_id;
    int next_action_id;
};

struct NODE_DATA_ILIFETIME {
    union NODE_CHILDREN_ILIFETIME {
        struct NODE_CHILDREN_ILIFETIME_STRUCT {
            node_st *begin;
            node_st *end;
            node_st *next;
        } ilifetime_children_st;

        node_st *ilifetime_children_at[3];
    } ilifetime_children;

    enum lifetime_type type;
};

struct NODE_DATA_NODESET_CHILD_ENTRY {
    union NODE_CHILDREN_NODESET_CHILD_ENTRY {
        struct NODE_CHILDREN_NODESET_CHILD_ENTRY_STRUCT {
            node_st *next;
        } nodeset_child_entry_children_st;

        node_st *nodeset_child_entry_children_at[1];
    } nodeset_child_entry_children;

    node_st *reference;
};

struct NODE_DATA_INODESET {
    union NODE_CHILDREN_INODESET {
        struct NODE_CHILDREN_INODESET_STRUCT {
            node_st *name;
            node_st *expr;
            node_st *iattributes;
            node_st *unpacked;
            node_st *next;
            node_st *children_table;
        } inodeset_children_st;

        node_st *inodeset_children_at[6];
    } inodeset_children;

    char * iinfo;
    bool illegal_setexpr_attr;
};

struct NODE_DATA_INODE {
    union NODE_CHILDREN_INODE {
        struct NODE_CHILDREN_INODE_STRUCT {
            node_st *name;
            node_st *next;
            node_st *ichildren;
            node_st *iattributes;
            node_st *iequations;
            node_st *lifetimes;
            node_st *visit;
        } inode_children_st;

        node_st *inode_children_at[7];
    } inode_children;

    char * iifno;
    int is_root;
    int index;
};

struct NODE_DATA_IPASS {
    union NODE_CHILDREN_IPASS {
        struct NODE_CHILDREN_IPASS_STRUCT {
            node_st *name;
            node_st *iprefix;
            node_st *target_func;
            node_st *next;
        } ipass_children_st;

        node_st *ipass_children_at[4];
    } ipass_children;

    char * iifno;
};

struct NODE_DATA_ITRAVERSAL {
    union NODE_CHILDREN_ITRAVERSAL {
        struct NODE_CHILDREN_ITRAVERSAL_STRUCT {
            node_st *name;
            node_st *iprefix;
            node_st *inodes;
            node_st *data;
            node_st *next;
        } itraversal_children_st;

        node_st *itraversal_children_at[5];
    } itraversal_children;

    int index;
    char * iinfo;
};

struct NODE_DATA_IPHASE {
    union NODE_CHILDREN_IPHASE {
        struct NODE_CHILDREN_IPHASE_STRUCT {
            node_st *name;
            node_st *iprefix;
            node_st *gate_func;
            node_st *iactions;
            node_st *next;
        } iphase_children_st;

        node_st *iphase_children_at[5];
    } iphase_children;

    int is_start;
    int is_cycle;
    char * iinfo;
};

struct NODE_DATA_IACTIONS {
    union NODE_CHILDREN_IACTIONS {
        struct NODE_CHILDREN_IACTIONS_STRUCT {
            node_st *reference;
            node_st *next;
        } iactions_children_st;

        node_st *iactions_children_at[2];
    } iactions_children;

    int action_id;
};

struct NODE_DATA_AST {
    union NODE_CHILDREN_AST {
        struct NODE_CHILDREN_AST_STRUCT {
            node_st *iphases;
            node_st *itraversals;
            node_st *ipasses;
            node_st *inodes;
            node_st *inodesets;
            node_st *enums;
            node_st *stable;
        } ast_children_st;

        node_st *ast_children_at[7];
    } ast_children;

    int num_traversals;
    int num_nodes;
    node_st *root_node;
    node_st *start_phase;
    bool uses_unsafe;
};

#define ID_NEXT(n) ((n)->data.N_id->id_children.id_children_st.next)
#define ID_ORIG(n) ((n)->data.N_id->orig)
#define ID_LWR(n) ((n)->data.N_id->lwr)
#define ID_UPR(n) ((n)->data.N_id->upr)
#define ID_ROW(n) ((n)->data.N_id->row)
#define ID_COL_BEGIN(n) ((n)->data.N_id->col_begin)
#define ID_COL_END(n) ((n)->data.N_id->col_end)
#define IENUM_VALS(n) ((n)->data.N_ienum->ienum_children.ienum_children_st.vals)
#define IENUM_NAME(n) ((n)->data.N_ienum->ienum_children.ienum_children_st.name)
#define IENUM_IPREFIX(n) ((n)->data.N_ienum->ienum_children.ienum_children_st.iprefix)
#define IENUM_NEXT(n) ((n)->data.N_ienum->ienum_children.ienum_children_st.next)
#define IENUM_IINFO(n) ((n)->data.N_ienum->iinfo)
#define VISIT_SEQUENCE_VISIT_ALT(n) ((n)->data.N_visit_sequence_visit->visit_sequence_visit_children.visit_sequence_visit_children_st.alt)
#define VISIT_SEQUENCE_VISIT_NEXT(n) ((n)->data.N_visit_sequence_visit->visit_sequence_visit_children.visit_sequence_visit_children_st.next)
#define VISIT_SEQUENCE_VISIT_CHILD(n) ((n)->data.N_visit_sequence_visit->child)
#define VISIT_SEQUENCE_VISIT_VISIT(n) ((n)->data.N_visit_sequence_visit->visit)
#define VISIT_SEQUENCE_EVAL_NEXT(n) ((n)->data.N_visit_sequence_eval->visit_sequence_eval_children.visit_sequence_eval_children_st.next)
#define VISIT_SEQUENCE_EVAL_ATTRIBUTE(n) ((n)->data.N_visit_sequence_eval->attribute)
#define VISIT_SEQUENCE(n) ((n)->data.N_visit->visit_children.visit_children_st.sequence)
#define VISIT_INPUTS(n) ((n)->data.N_visit->visit_children.visit_children_st.inputs)
#define VISIT_OUTPUTS(n) ((n)->data.N_visit->visit_children.visit_children_st.outputs)
#define VISIT_NEXT(n) ((n)->data.N_visit->visit_children.visit_children_st.next)
#define VISIT_INODE(n) ((n)->data.N_visit->inode)
#define VISIT_INDEX(n) ((n)->data.N_visit->index)
#define VISIT_ARG_LIST_ATTRIBUTE(n) ((n)->data.N_visit_arg_list->visit_arg_list_children.visit_arg_list_children_st.attribute)
#define VISIT_ARG_LIST_NEXT(n) ((n)->data.N_visit_arg_list->visit_arg_list_children.visit_arg_list_children_st.next)
#define ATTRIBUTE_REFERENCE_INODE(n) ((n)->data.N_attribute_reference->attribute_reference_children.attribute_reference_children_st.inode)
#define ATTRIBUTE_REFERENCE_IATTRIBUTE(n) ((n)->data.N_attribute_reference->attribute_reference_children.attribute_reference_children_st.iattribute)
#define ATTRIBUTE_REFERENCE_NODE_TYPE(n) ((n)->data.N_attribute_reference->node_type)
#define ATTRIBUTE_REFERENCE_REFERENCE(n) ((n)->data.N_attribute_reference->reference)
#define ATTRIBUTE_REFERENCE_CHILD_REFERENCE(n) ((n)->data.N_attribute_reference->child_reference)
#define EQUATION_DEPENDENCY_IATTRIBUTE(n) ((n)->data.N_equation_dependency->equation_dependency_children.equation_dependency_children_st.iattribute)
#define EQUATION_DEPENDENCY_NEXT(n) ((n)->data.N_equation_dependency->equation_dependency_children.equation_dependency_children_st.next)
#define EQUATION_RULE(n) ((n)->data.N_equation->equation_children.equation_children_st.rule)
#define EQUATION_IARGS(n) ((n)->data.N_equation->equation_children.equation_children_st.iargs)
#define EQUATION_NEXT(n) ((n)->data.N_equation->equation_children.equation_children_st.next)
#define ATTRIBUTE_NAME(n) ((n)->data.N_attribute->attribute_children.attribute_children_st.name)
#define ATTRIBUTE_TYPE_REFERENCE(n) ((n)->data.N_attribute->attribute_children.attribute_children_st.type_reference)
#define ATTRIBUTE_LIFETIMES(n) ((n)->data.N_attribute->attribute_children.attribute_children_st.lifetimes)
#define ATTRIBUTE_NEXT(n) ((n)->data.N_attribute->attribute_children.attribute_children_st.next)
#define ATTRIBUTE_TYPE(n) ((n)->data.N_attribute->type)
#define ATTRIBUTE_IN_CONSTRUCTOR(n) ((n)->data.N_attribute->in_constructor)
#define ATTRIBUTE_IS_INHERITED(n) ((n)->data.N_attribute->is_inherited)
#define ATTRIBUTE_IS_SYNTHESIZED(n) ((n)->data.N_attribute->is_synthesized)
#define ATTRIBUTE_IS_PROPAGATED(n) ((n)->data.N_attribute->is_propagated)
#define ITRAVDATA_NAME(n) ((n)->data.N_itravdata->itravdata_children.itravdata_children_st.name)
#define ITRAVDATA_TYPE_REFERENCE(n) ((n)->data.N_itravdata->itravdata_children.itravdata_children_st.type_reference)
#define ITRAVDATA_NEXT(n) ((n)->data.N_itravdata->itravdata_children.itravdata_children_st.next)
#define ITRAVDATA_TYPE(n) ((n)->data.N_itravdata->type)
#define ITRAVDATA_INCLUDE_FILE(n) ((n)->data.N_itravdata->include_file)
#define SETOPERATION_LEFT(n) ((n)->data.N_setoperation->setoperation_children.setoperation_children_st.left)
#define SETOPERATION_RIGHT(n) ((n)->data.N_setoperation->setoperation_children.setoperation_children_st.right)
#define SETOPERATION_TYPE(n) ((n)->data.N_setoperation->type)
#define SETLITERAL_REFERENCE(n) ((n)->data.N_setliteral->setliteral_children.setliteral_children_st.reference)
#define SETLITERAL_LEFT(n) ((n)->data.N_setliteral->setliteral_children.setliteral_children_st.left)
#define SETLITERAL_RIGHT(n) ((n)->data.N_setliteral->setliteral_children.setliteral_children_st.right)
#define SETREFERENCE_REFERENCE(n) ((n)->data.N_setreference->setreference_children.setreference_children_st.reference)
#define STE_NEXT(n) ((n)->data.N_ste->ste_children.ste_children_st.next)
#define STE_KEY(n) ((n)->data.N_ste->key)
#define STE_VALUE(n) ((n)->data.N_ste->value)
#define CHILD_NAME(n) ((n)->data.N_child->child_children.child_children_st.name)
#define CHILD_LIFETIMES(n) ((n)->data.N_child->child_children.child_children_st.lifetimes)
#define CHILD_NEXT(n) ((n)->data.N_child->child_children.child_children_st.next)
#define CHILD_TYPE_REFERENCE(n) ((n)->data.N_child->child_children.child_children_st.type_reference)
#define CHILD_TYPE(n) ((n)->data.N_child->type)
#define CHILD_IN_CONSTRUCTOR(n) ((n)->data.N_child->in_constructor)
#define CHILD_IS_MANDATORY(n) ((n)->data.N_child->is_mandatory)
#define LIFETIME_RANGE_TARGET(n) ((n)->data.N_lifetime_range->lifetime_range_children.lifetime_range_children_st.target)
#define LIFETIME_RANGE_INCLUSIVE(n) ((n)->data.N_lifetime_range->inclusive)
#define LIFETIME_RANGE_ACTION_ID(n) ((n)->data.N_lifetime_range->action_id)
#define LIFETIME_RANGE_NEXT_ACTION_ID(n) ((n)->data.N_lifetime_range->next_action_id)
#define ILIFETIME_BEGIN(n) ((n)->data.N_ilifetime->ilifetime_children.ilifetime_children_st.begin)
#define ILIFETIME_END(n) ((n)->data.N_ilifetime->ilifetime_children.ilifetime_children_st.end)
#define ILIFETIME_NEXT(n) ((n)->data.N_ilifetime->ilifetime_children.ilifetime_children_st.next)
#define ILIFETIME_TYPE(n) ((n)->data.N_ilifetime->type)
#define NODESET_CHILD_ENTRY_NEXT(n) ((n)->data.N_nodeset_child_entry->nodeset_child_entry_children.nodeset_child_entry_children_st.next)
#define NODESET_CHILD_ENTRY_REFERENCE(n) ((n)->data.N_nodeset_child_entry->reference)
#define INODESET_NAME(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.name)
#define INODESET_EXPR(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.expr)
#define INODESET_IATTRIBUTES(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.iattributes)
#define INODESET_UNPACKED(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.unpacked)
#define INODESET_NEXT(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.next)
#define INODESET_CHILDREN_TABLE(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.children_table)
#define INODESET_IINFO(n) ((n)->data.N_inodeset->iinfo)
#define INODESET_ILLEGAL_SETEXPR_ATTR(n) ((n)->data.N_inodeset->illegal_setexpr_attr)
#define INODE_NAME(n) ((n)->data.N_inode->inode_children.inode_children_st.name)
#define INODE_NEXT(n) ((n)->data.N_inode->inode_children.inode_children_st.next)
#define INODE_ICHILDREN(n) ((n)->data.N_inode->inode_children.inode_children_st.ichildren)
#define INODE_IATTRIBUTES(n) ((n)->data.N_inode->inode_children.inode_children_st.iattributes)
#define INODE_IEQUATIONS(n) ((n)->data.N_inode->inode_children.inode_children_st.iequations)
#define INODE_LIFETIMES(n) ((n)->data.N_inode->inode_children.inode_children_st.lifetimes)
#define INODE_VISIT(n) ((n)->data.N_inode->inode_children.inode_children_st.visit)
#define INODE_IIFNO(n) ((n)->data.N_inode->iifno)
#define INODE_IS_ROOT(n) ((n)->data.N_inode->is_root)
#define INODE_INDEX(n) ((n)->data.N_inode->index)
#define IPASS_NAME(n) ((n)->data.N_ipass->ipass_children.ipass_children_st.name)
#define IPASS_IPREFIX(n) ((n)->data.N_ipass->ipass_children.ipass_children_st.iprefix)
#define IPASS_TARGET_FUNC(n) ((n)->data.N_ipass->ipass_children.ipass_children_st.target_func)
#define IPASS_NEXT(n) ((n)->data.N_ipass->ipass_children.ipass_children_st.next)
#define IPASS_IIFNO(n) ((n)->data.N_ipass->iifno)
#define ITRAVERSAL_NAME(n) ((n)->data.N_itraversal->itraversal_children.itraversal_children_st.name)
#define ITRAVERSAL_IPREFIX(n) ((n)->data.N_itraversal->itraversal_children.itraversal_children_st.iprefix)
#define ITRAVERSAL_INODES(n) ((n)->data.N_itraversal->itraversal_children.itraversal_children_st.inodes)
#define ITRAVERSAL_DATA(n) ((n)->data.N_itraversal->itraversal_children.itraversal_children_st.data)
#define ITRAVERSAL_NEXT(n) ((n)->data.N_itraversal->itraversal_children.itraversal_children_st.next)
#define ITRAVERSAL_INDEX(n) ((n)->data.N_itraversal->index)
#define ITRAVERSAL_IINFO(n) ((n)->data.N_itraversal->iinfo)
#define IPHASE_NAME(n) ((n)->data.N_iphase->iphase_children.iphase_children_st.name)
#define IPHASE_IPREFIX(n) ((n)->data.N_iphase->iphase_children.iphase_children_st.iprefix)
#define IPHASE_GATE_FUNC(n) ((n)->data.N_iphase->iphase_children.iphase_children_st.gate_func)
#define IPHASE_IACTIONS(n) ((n)->data.N_iphase->iphase_children.iphase_children_st.iactions)
#define IPHASE_NEXT(n) ((n)->data.N_iphase->iphase_children.iphase_children_st.next)
#define IPHASE_IS_START(n) ((n)->data.N_iphase->is_start)
#define IPHASE_IS_CYCLE(n) ((n)->data.N_iphase->is_cycle)
#define IPHASE_IINFO(n) ((n)->data.N_iphase->iinfo)
#define IACTIONS_REFERENCE(n) ((n)->data.N_iactions->iactions_children.iactions_children_st.reference)
#define IACTIONS_NEXT(n) ((n)->data.N_iactions->iactions_children.iactions_children_st.next)
#define IACTIONS_ACTION_ID(n) ((n)->data.N_iactions->action_id)
#define AST_IPHASES(n) ((n)->data.N_ast->ast_children.ast_children_st.iphases)
#define AST_ITRAVERSALS(n) ((n)->data.N_ast->ast_children.ast_children_st.itraversals)
#define AST_IPASSES(n) ((n)->data.N_ast->ast_children.ast_children_st.ipasses)
#define AST_INODES(n) ((n)->data.N_ast->ast_children.ast_children_st.inodes)
#define AST_INODESETS(n) ((n)->data.N_ast->ast_children.ast_children_st.inodesets)
#define AST_ENUMS(n) ((n)->data.N_ast->ast_children.ast_children_st.enums)
#define AST_STABLE(n) ((n)->data.N_ast->ast_children.ast_children_st.stable)
#define AST_NUM_TRAVERSALS(n) ((n)->data.N_ast->num_traversals)
#define AST_NUM_NODES(n) ((n)->data.N_ast->num_nodes)
#define AST_ROOT_NODE(n) ((n)->data.N_ast->root_node)
#define AST_START_PHASE(n) ((n)->data.N_ast->start_phase)
#define AST_USES_UNSAFE(n) ((n)->data.N_ast->uses_unsafe)
node_st *ASTid(char * orig, char * lwr, char * Upr);
node_st *ASTienum(node_st *vals, node_st *name, node_st *iprefix, char * iinfo);
node_st *ASTvisit_sequence_visit(node_st *child, node_st *visit);
node_st *ASTvisit_sequence_eval(node_st *attribute);
node_st *ASTvisit(node_st *sequence, node_st *inputs, node_st *outputs, node_st *inode, uint64_t index);
node_st *ASTvisit_arg_list(node_st *attribute);
node_st *ASTattribute_reference(void);
node_st *ASTequation_dependency(void);
node_st *ASTequation(void);
node_st *ASTattribute(void);
node_st *ASTitravdata(node_st *name);
node_st *ASTsetoperation(node_st *left, node_st *right, enum setoperation_type type);
node_st *ASTsetliteral(node_st *reference);
node_st *ASTsetreference(void);
node_st *ASTste(void);
node_st *ASTchild(node_st *name);
node_st *ASTlifetime_range(void);
node_st *ASTilifetime(void);
node_st *ASTnodeset_child_entry(node_st *reference);
node_st *ASTinodeset(void);
node_st *ASTinode(node_st *name, char * iifno);
node_st *ASTipass(node_st *name, char * iifno);
node_st *ASTitraversal(node_st *name);
node_st *ASTiphase(node_st *name, int is_start);
node_st *ASTiactions(void);
node_st *ASTast(void);
union NODE_DATA {
    struct NODE_DATA_AST *N_ast;
    struct NODE_DATA_IACTIONS *N_iactions;
    struct NODE_DATA_IPHASE *N_iphase;
    struct NODE_DATA_ITRAVERSAL *N_itraversal;
    struct NODE_DATA_IPASS *N_ipass;
    struct NODE_DATA_INODE *N_inode;
    struct NODE_DATA_INODESET *N_inodeset;
    struct NODE_DATA_NODESET_CHILD_ENTRY *N_nodeset_child_entry;
    struct NODE_DATA_ILIFETIME *N_ilifetime;
    struct NODE_DATA_LIFETIME_RANGE *N_lifetime_range;
    struct NODE_DATA_CHILD *N_child;
    struct NODE_DATA_STE *N_ste;
    struct NODE_DATA_SETREFERENCE *N_setreference;
    struct NODE_DATA_SETLITERAL *N_setliteral;
    struct NODE_DATA_SETOPERATION *N_setoperation;
    struct NODE_DATA_ITRAVDATA *N_itravdata;
    struct NODE_DATA_ATTRIBUTE *N_attribute;
    struct NODE_DATA_EQUATION *N_equation;
    struct NODE_DATA_EQUATION_DEPENDENCY *N_equation_dependency;
    struct NODE_DATA_ATTRIBUTE_REFERENCE *N_attribute_reference;
    struct NODE_DATA_VISIT_ARG_LIST *N_visit_arg_list;
    struct NODE_DATA_VISIT *N_visit;
    struct NODE_DATA_VISIT_SEQUENCE_EVAL *N_visit_sequence_eval;
    struct NODE_DATA_VISIT_SEQUENCE_VISIT *N_visit_sequence_visit;
    struct NODE_DATA_IENUM *N_ienum;
    struct NODE_DATA_ID *N_id;
};

#define NODE_TYPE(n) ((n)->nodetype)
#define NODE_CHILDREN(n) ((n)->children)
#define NODE_NUMCHILDREN(n) ((n)->num_children)
#define NODE_FILENAME(n) ((n)->filename)
#define NODE_BLINE(n) ((n)->begin_line)
#define NODE_ELINE(n) ((n)->end_line)
#define NODE_BCOL(n) ((n)->begin_col)
#define NODE_ECOL(n) ((n)->end_col)
typedef struct ccn_node {
    enum ccn_nodetype nodetype;
    union NODE_DATA data;
    struct ccn_node **children;
    char *filename;
    long int num_children;
    uint32_t begin_line;
    uint32_t end_line;
    uint32_t begin_col;
    uint32_t end_col;
} ccn_node;

