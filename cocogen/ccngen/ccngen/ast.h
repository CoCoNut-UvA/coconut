#pragma once
#include <stddef.h>

#include "ccngen/ccn_defs.h"
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

struct NODE_DATA_VISIT_SEQUENCE_DUMMY {
    union NODE_CHILDREN_VISIT_SEQUENCE_DUMMY {
        struct NODE_CHILDREN_VISIT_SEQUENCE_DUMMY_STRUCT {
            node_st *alt;
        } visit_sequence_dummy_children_st;

        node_st *visit_sequence_dummy_children_at[1];
    } visit_sequence_dummy_children;

    node_st *inode;
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
    bool in_constructor;
    bool is_mandatory;
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
    bool is_root;
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

    bool is_start;
    bool is_cycle;
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

#define ID_NEXT(n) (*CCNaccess_id_next(__LINE__, __FILE__, __func__, n))
#define ID_ORIG(n) (*CCNaccess_id_orig(__LINE__, __FILE__, __func__, n))
#define ID_LWR(n) (*CCNaccess_id_lwr(__LINE__, __FILE__, __func__, n))
#define ID_UPR(n) (*CCNaccess_id_upr(__LINE__, __FILE__, __func__, n))
#define ID_ROW(n) (*CCNaccess_id_row(__LINE__, __FILE__, __func__, n))
#define ID_COL_BEGIN(n) (*CCNaccess_id_col_begin(__LINE__, __FILE__, __func__, n))
#define ID_COL_END(n) (*CCNaccess_id_col_end(__LINE__, __FILE__, __func__, n))
#define IENUM_VALS(n) (*CCNaccess_ienum_vals(__LINE__, __FILE__, __func__, n))
#define IENUM_NAME(n) (*CCNaccess_ienum_name(__LINE__, __FILE__, __func__, n))
#define IENUM_IPREFIX(n) (*CCNaccess_ienum_iprefix(__LINE__, __FILE__, __func__, n))
#define IENUM_NEXT(n) (*CCNaccess_ienum_next(__LINE__, __FILE__, __func__, n))
#define IENUM_IINFO(n) (*CCNaccess_ienum_iinfo(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE_DUMMY_ALT(n) (*CCNaccess_visit_sequence_dummy_alt(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE_DUMMY_INODE(n) (*CCNaccess_visit_sequence_dummy_inode(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE_VISIT_ALT(n) (*CCNaccess_visit_sequence_visit_alt(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE_VISIT_NEXT(n) (*CCNaccess_visit_sequence_visit_next(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE_VISIT_CHILD(n) (*CCNaccess_visit_sequence_visit_child(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE_VISIT_VISIT(n) (*CCNaccess_visit_sequence_visit_visit(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE_EVAL_NEXT(n) (*CCNaccess_visit_sequence_eval_next(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE_EVAL_ATTRIBUTE(n) (*CCNaccess_visit_sequence_eval_attribute(__LINE__, __FILE__, __func__, n))
#define VISIT_SEQUENCE(n) (*CCNaccess_visit_sequence(__LINE__, __FILE__, __func__, n))
#define VISIT_INPUTS(n) (*CCNaccess_visit_inputs(__LINE__, __FILE__, __func__, n))
#define VISIT_OUTPUTS(n) (*CCNaccess_visit_outputs(__LINE__, __FILE__, __func__, n))
#define VISIT_NEXT(n) (*CCNaccess_visit_next(__LINE__, __FILE__, __func__, n))
#define VISIT_INODE(n) (*CCNaccess_visit_inode(__LINE__, __FILE__, __func__, n))
#define VISIT_INDEX(n) (*CCNaccess_visit_index(__LINE__, __FILE__, __func__, n))
#define VISIT_ARG_LIST_ATTRIBUTE(n) (*CCNaccess_visit_arg_list_attribute(__LINE__, __FILE__, __func__, n))
#define VISIT_ARG_LIST_NEXT(n) (*CCNaccess_visit_arg_list_next(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_REFERENCE_INODE(n) (*CCNaccess_attribute_reference_inode(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_REFERENCE_IATTRIBUTE(n) (*CCNaccess_attribute_reference_iattribute(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_REFERENCE_NODE_TYPE(n) (*CCNaccess_attribute_reference_node_type(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_REFERENCE_REFERENCE(n) (*CCNaccess_attribute_reference_reference(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_REFERENCE_CHILD_REFERENCE(n) (*CCNaccess_attribute_reference_child_reference(__LINE__, __FILE__, __func__, n))
#define EQUATION_DEPENDENCY_IATTRIBUTE(n) (*CCNaccess_equation_dependency_iattribute(__LINE__, __FILE__, __func__, n))
#define EQUATION_DEPENDENCY_NEXT(n) (*CCNaccess_equation_dependency_next(__LINE__, __FILE__, __func__, n))
#define EQUATION_RULE(n) (*CCNaccess_equation_rule(__LINE__, __FILE__, __func__, n))
#define EQUATION_IARGS(n) (*CCNaccess_equation_iargs(__LINE__, __FILE__, __func__, n))
#define EQUATION_NEXT(n) (*CCNaccess_equation_next(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_NAME(n) (*CCNaccess_attribute_name(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_TYPE_REFERENCE(n) (*CCNaccess_attribute_type_reference(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_LIFETIMES(n) (*CCNaccess_attribute_lifetimes(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_NEXT(n) (*CCNaccess_attribute_next(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_TYPE(n) (*CCNaccess_attribute_type(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_IN_CONSTRUCTOR(n) (*CCNaccess_attribute_in_constructor(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_IS_INHERITED(n) (*CCNaccess_attribute_is_inherited(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_IS_SYNTHESIZED(n) (*CCNaccess_attribute_is_synthesized(__LINE__, __FILE__, __func__, n))
#define ATTRIBUTE_IS_PROPAGATED(n) (*CCNaccess_attribute_is_propagated(__LINE__, __FILE__, __func__, n))
#define ITRAVDATA_NAME(n) (*CCNaccess_itravdata_name(__LINE__, __FILE__, __func__, n))
#define ITRAVDATA_TYPE_REFERENCE(n) (*CCNaccess_itravdata_type_reference(__LINE__, __FILE__, __func__, n))
#define ITRAVDATA_NEXT(n) (*CCNaccess_itravdata_next(__LINE__, __FILE__, __func__, n))
#define ITRAVDATA_TYPE(n) (*CCNaccess_itravdata_type(__LINE__, __FILE__, __func__, n))
#define ITRAVDATA_INCLUDE_FILE(n) (*CCNaccess_itravdata_include_file(__LINE__, __FILE__, __func__, n))
#define SETOPERATION_LEFT(n) (*CCNaccess_setoperation_left(__LINE__, __FILE__, __func__, n))
#define SETOPERATION_RIGHT(n) (*CCNaccess_setoperation_right(__LINE__, __FILE__, __func__, n))
#define SETOPERATION_TYPE(n) (*CCNaccess_setoperation_type(__LINE__, __FILE__, __func__, n))
#define SETLITERAL_REFERENCE(n) (*CCNaccess_setliteral_reference(__LINE__, __FILE__, __func__, n))
#define SETLITERAL_LEFT(n) (*CCNaccess_setliteral_left(__LINE__, __FILE__, __func__, n))
#define SETLITERAL_RIGHT(n) (*CCNaccess_setliteral_right(__LINE__, __FILE__, __func__, n))
#define SETREFERENCE_REFERENCE(n) (*CCNaccess_setreference_reference(__LINE__, __FILE__, __func__, n))
#define STE_NEXT(n) (*CCNaccess_ste_next(__LINE__, __FILE__, __func__, n))
#define STE_KEY(n) (*CCNaccess_ste_key(__LINE__, __FILE__, __func__, n))
#define STE_VALUE(n) (*CCNaccess_ste_value(__LINE__, __FILE__, __func__, n))
#define CHILD_NAME(n) (*CCNaccess_child_name(__LINE__, __FILE__, __func__, n))
#define CHILD_LIFETIMES(n) (*CCNaccess_child_lifetimes(__LINE__, __FILE__, __func__, n))
#define CHILD_NEXT(n) (*CCNaccess_child_next(__LINE__, __FILE__, __func__, n))
#define CHILD_TYPE_REFERENCE(n) (*CCNaccess_child_type_reference(__LINE__, __FILE__, __func__, n))
#define CHILD_TYPE(n) (*CCNaccess_child_type(__LINE__, __FILE__, __func__, n))
#define CHILD_IN_CONSTRUCTOR(n) (*CCNaccess_child_in_constructor(__LINE__, __FILE__, __func__, n))
#define CHILD_IS_MANDATORY(n) (*CCNaccess_child_is_mandatory(__LINE__, __FILE__, __func__, n))
#define LIFETIME_RANGE_TARGET(n) (*CCNaccess_lifetime_range_target(__LINE__, __FILE__, __func__, n))
#define LIFETIME_RANGE_INCLUSIVE(n) (*CCNaccess_lifetime_range_inclusive(__LINE__, __FILE__, __func__, n))
#define LIFETIME_RANGE_ACTION_ID(n) (*CCNaccess_lifetime_range_action_id(__LINE__, __FILE__, __func__, n))
#define LIFETIME_RANGE_NEXT_ACTION_ID(n) (*CCNaccess_lifetime_range_next_action_id(__LINE__, __FILE__, __func__, n))
#define ILIFETIME_BEGIN(n) (*CCNaccess_ilifetime_begin(__LINE__, __FILE__, __func__, n))
#define ILIFETIME_END(n) (*CCNaccess_ilifetime_end(__LINE__, __FILE__, __func__, n))
#define ILIFETIME_NEXT(n) (*CCNaccess_ilifetime_next(__LINE__, __FILE__, __func__, n))
#define ILIFETIME_TYPE(n) (*CCNaccess_ilifetime_type(__LINE__, __FILE__, __func__, n))
#define NODESET_CHILD_ENTRY_NEXT(n) (*CCNaccess_nodeset_child_entry_next(__LINE__, __FILE__, __func__, n))
#define NODESET_CHILD_ENTRY_REFERENCE(n) (*CCNaccess_nodeset_child_entry_reference(__LINE__, __FILE__, __func__, n))
#define INODESET_NAME(n) (*CCNaccess_inodeset_name(__LINE__, __FILE__, __func__, n))
#define INODESET_EXPR(n) (*CCNaccess_inodeset_expr(__LINE__, __FILE__, __func__, n))
#define INODESET_IATTRIBUTES(n) (*CCNaccess_inodeset_iattributes(__LINE__, __FILE__, __func__, n))
#define INODESET_UNPACKED(n) (*CCNaccess_inodeset_unpacked(__LINE__, __FILE__, __func__, n))
#define INODESET_NEXT(n) (*CCNaccess_inodeset_next(__LINE__, __FILE__, __func__, n))
#define INODESET_CHILDREN_TABLE(n) (*CCNaccess_inodeset_children_table(__LINE__, __FILE__, __func__, n))
#define INODESET_IINFO(n) (*CCNaccess_inodeset_iinfo(__LINE__, __FILE__, __func__, n))
#define INODESET_ILLEGAL_SETEXPR_ATTR(n) (*CCNaccess_inodeset_illegal_setexpr_attr(__LINE__, __FILE__, __func__, n))
#define INODE_NAME(n) (*CCNaccess_inode_name(__LINE__, __FILE__, __func__, n))
#define INODE_NEXT(n) (*CCNaccess_inode_next(__LINE__, __FILE__, __func__, n))
#define INODE_ICHILDREN(n) (*CCNaccess_inode_ichildren(__LINE__, __FILE__, __func__, n))
#define INODE_IATTRIBUTES(n) (*CCNaccess_inode_iattributes(__LINE__, __FILE__, __func__, n))
#define INODE_IEQUATIONS(n) (*CCNaccess_inode_iequations(__LINE__, __FILE__, __func__, n))
#define INODE_LIFETIMES(n) (*CCNaccess_inode_lifetimes(__LINE__, __FILE__, __func__, n))
#define INODE_VISIT(n) (*CCNaccess_inode_visit(__LINE__, __FILE__, __func__, n))
#define INODE_IIFNO(n) (*CCNaccess_inode_iifno(__LINE__, __FILE__, __func__, n))
#define INODE_IS_ROOT(n) (*CCNaccess_inode_is_root(__LINE__, __FILE__, __func__, n))
#define INODE_INDEX(n) (*CCNaccess_inode_index(__LINE__, __FILE__, __func__, n))
#define IPASS_NAME(n) (*CCNaccess_ipass_name(__LINE__, __FILE__, __func__, n))
#define IPASS_IPREFIX(n) (*CCNaccess_ipass_iprefix(__LINE__, __FILE__, __func__, n))
#define IPASS_TARGET_FUNC(n) (*CCNaccess_ipass_target_func(__LINE__, __FILE__, __func__, n))
#define IPASS_NEXT(n) (*CCNaccess_ipass_next(__LINE__, __FILE__, __func__, n))
#define IPASS_IIFNO(n) (*CCNaccess_ipass_iifno(__LINE__, __FILE__, __func__, n))
#define ITRAVERSAL_NAME(n) (*CCNaccess_itraversal_name(__LINE__, __FILE__, __func__, n))
#define ITRAVERSAL_IPREFIX(n) (*CCNaccess_itraversal_iprefix(__LINE__, __FILE__, __func__, n))
#define ITRAVERSAL_INODES(n) (*CCNaccess_itraversal_inodes(__LINE__, __FILE__, __func__, n))
#define ITRAVERSAL_DATA(n) (*CCNaccess_itraversal_data(__LINE__, __FILE__, __func__, n))
#define ITRAVERSAL_NEXT(n) (*CCNaccess_itraversal_next(__LINE__, __FILE__, __func__, n))
#define ITRAVERSAL_INDEX(n) (*CCNaccess_itraversal_index(__LINE__, __FILE__, __func__, n))
#define ITRAVERSAL_IINFO(n) (*CCNaccess_itraversal_iinfo(__LINE__, __FILE__, __func__, n))
#define IPHASE_NAME(n) (*CCNaccess_iphase_name(__LINE__, __FILE__, __func__, n))
#define IPHASE_IPREFIX(n) (*CCNaccess_iphase_iprefix(__LINE__, __FILE__, __func__, n))
#define IPHASE_GATE_FUNC(n) (*CCNaccess_iphase_gate_func(__LINE__, __FILE__, __func__, n))
#define IPHASE_IACTIONS(n) (*CCNaccess_iphase_iactions(__LINE__, __FILE__, __func__, n))
#define IPHASE_NEXT(n) (*CCNaccess_iphase_next(__LINE__, __FILE__, __func__, n))
#define IPHASE_IS_START(n) (*CCNaccess_iphase_is_start(__LINE__, __FILE__, __func__, n))
#define IPHASE_IS_CYCLE(n) (*CCNaccess_iphase_is_cycle(__LINE__, __FILE__, __func__, n))
#define IPHASE_IINFO(n) (*CCNaccess_iphase_iinfo(__LINE__, __FILE__, __func__, n))
#define IACTIONS_REFERENCE(n) (*CCNaccess_iactions_reference(__LINE__, __FILE__, __func__, n))
#define IACTIONS_NEXT(n) (*CCNaccess_iactions_next(__LINE__, __FILE__, __func__, n))
#define IACTIONS_ACTION_ID(n) (*CCNaccess_iactions_action_id(__LINE__, __FILE__, __func__, n))
#define AST_IPHASES(n) (*CCNaccess_ast_iphases(__LINE__, __FILE__, __func__, n))
#define AST_ITRAVERSALS(n) (*CCNaccess_ast_itraversals(__LINE__, __FILE__, __func__, n))
#define AST_IPASSES(n) (*CCNaccess_ast_ipasses(__LINE__, __FILE__, __func__, n))
#define AST_INODES(n) (*CCNaccess_ast_inodes(__LINE__, __FILE__, __func__, n))
#define AST_INODESETS(n) (*CCNaccess_ast_inodesets(__LINE__, __FILE__, __func__, n))
#define AST_ENUMS(n) (*CCNaccess_ast_enums(__LINE__, __FILE__, __func__, n))
#define AST_STABLE(n) (*CCNaccess_ast_stable(__LINE__, __FILE__, __func__, n))
#define AST_NUM_TRAVERSALS(n) (*CCNaccess_ast_num_traversals(__LINE__, __FILE__, __func__, n))
#define AST_NUM_NODES(n) (*CCNaccess_ast_num_nodes(__LINE__, __FILE__, __func__, n))
#define AST_ROOT_NODE(n) (*CCNaccess_ast_root_node(__LINE__, __FILE__, __func__, n))
#define AST_START_PHASE(n) (*CCNaccess_ast_start_phase(__LINE__, __FILE__, __func__, n))
#define AST_USES_UNSAFE(n) (*CCNaccess_ast_uses_unsafe(__LINE__, __FILE__, __func__, n))
node_st *ASTid(char * orig, char * lwr, char * Upr);
node_st *ASTienum(node_st *vals, node_st *name, node_st *iprefix, char * iinfo);
node_st *ASTvisit_sequence_dummy(node_st *inode);
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
node_st *ASTiphase(node_st *name, bool is_start);
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
    struct NODE_DATA_VISIT_SEQUENCE_DUMMY *N_visit_sequence_dummy;
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

void CCNaccesserror_id_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_id_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ID) {
        CCNaccesserror_id_next(line, file, func);
    }

    #endif
    return &(node->data.N_id->id_children.id_children_st.next);
}

void CCNaccesserror_id_orig(int line, const char *file, const char *func);
static inline char * *CCNaccess_id_orig(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ID) {
        CCNaccesserror_id_orig(line, file, func);
    }

    #endif
    return &(node->data.N_id->orig);
}

void CCNaccesserror_id_lwr(int line, const char *file, const char *func);
static inline char * *CCNaccess_id_lwr(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ID) {
        CCNaccesserror_id_lwr(line, file, func);
    }

    #endif
    return &(node->data.N_id->lwr);
}

void CCNaccesserror_id_upr(int line, const char *file, const char *func);
static inline char * *CCNaccess_id_upr(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ID) {
        CCNaccesserror_id_upr(line, file, func);
    }

    #endif
    return &(node->data.N_id->upr);
}

void CCNaccesserror_id_row(int line, const char *file, const char *func);
static inline int *CCNaccess_id_row(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ID) {
        CCNaccesserror_id_row(line, file, func);
    }

    #endif
    return &(node->data.N_id->row);
}

void CCNaccesserror_id_col_begin(int line, const char *file, const char *func);
static inline int *CCNaccess_id_col_begin(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ID) {
        CCNaccesserror_id_col_begin(line, file, func);
    }

    #endif
    return &(node->data.N_id->col_begin);
}

void CCNaccesserror_id_col_end(int line, const char *file, const char *func);
static inline int *CCNaccess_id_col_end(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ID) {
        CCNaccesserror_id_col_end(line, file, func);
    }

    #endif
    return &(node->data.N_id->col_end);
}

void CCNaccesserror_ienum_vals(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ienum_vals(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IENUM) {
        CCNaccesserror_ienum_vals(line, file, func);
    }

    #endif
    return &(node->data.N_ienum->ienum_children.ienum_children_st.vals);
}

void CCNaccesserror_ienum_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ienum_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IENUM) {
        CCNaccesserror_ienum_name(line, file, func);
    }

    #endif
    return &(node->data.N_ienum->ienum_children.ienum_children_st.name);
}

void CCNaccesserror_ienum_iprefix(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ienum_iprefix(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IENUM) {
        CCNaccesserror_ienum_iprefix(line, file, func);
    }

    #endif
    return &(node->data.N_ienum->ienum_children.ienum_children_st.iprefix);
}

void CCNaccesserror_ienum_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ienum_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IENUM) {
        CCNaccesserror_ienum_next(line, file, func);
    }

    #endif
    return &(node->data.N_ienum->ienum_children.ienum_children_st.next);
}

void CCNaccesserror_ienum_iinfo(int line, const char *file, const char *func);
static inline char * *CCNaccess_ienum_iinfo(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IENUM) {
        CCNaccesserror_ienum_iinfo(line, file, func);
    }

    #endif
    return &(node->data.N_ienum->iinfo);
}

void CCNaccesserror_visit_sequence_dummy_alt(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_sequence_dummy_alt(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_SEQUENCE_DUMMY) {
        CCNaccesserror_visit_sequence_dummy_alt(line, file, func);
    }

    #endif
    return &(node->data.N_visit_sequence_dummy->visit_sequence_dummy_children.visit_sequence_dummy_children_st.alt);
}

void CCNaccesserror_visit_sequence_dummy_inode(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_visit_sequence_dummy_inode(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_SEQUENCE_DUMMY) {
        CCNaccesserror_visit_sequence_dummy_inode(line, file, func);
    }

    #endif
    return &(node->data.N_visit_sequence_dummy->inode);
}

void CCNaccesserror_visit_sequence_visit_alt(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_sequence_visit_alt(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_SEQUENCE_VISIT) {
        CCNaccesserror_visit_sequence_visit_alt(line, file, func);
    }

    #endif
    return &(node->data.N_visit_sequence_visit->visit_sequence_visit_children.visit_sequence_visit_children_st.alt);
}

void CCNaccesserror_visit_sequence_visit_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_sequence_visit_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_SEQUENCE_VISIT) {
        CCNaccesserror_visit_sequence_visit_next(line, file, func);
    }

    #endif
    return &(node->data.N_visit_sequence_visit->visit_sequence_visit_children.visit_sequence_visit_children_st.next);
}

void CCNaccesserror_visit_sequence_visit_child(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_visit_sequence_visit_child(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_SEQUENCE_VISIT) {
        CCNaccesserror_visit_sequence_visit_child(line, file, func);
    }

    #endif
    return &(node->data.N_visit_sequence_visit->child);
}

void CCNaccesserror_visit_sequence_visit_visit(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_visit_sequence_visit_visit(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_SEQUENCE_VISIT) {
        CCNaccesserror_visit_sequence_visit_visit(line, file, func);
    }

    #endif
    return &(node->data.N_visit_sequence_visit->visit);
}

void CCNaccesserror_visit_sequence_eval_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_sequence_eval_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_SEQUENCE_EVAL) {
        CCNaccesserror_visit_sequence_eval_next(line, file, func);
    }

    #endif
    return &(node->data.N_visit_sequence_eval->visit_sequence_eval_children.visit_sequence_eval_children_st.next);
}

void CCNaccesserror_visit_sequence_eval_attribute(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_visit_sequence_eval_attribute(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_SEQUENCE_EVAL) {
        CCNaccesserror_visit_sequence_eval_attribute(line, file, func);
    }

    #endif
    return &(node->data.N_visit_sequence_eval->attribute);
}

void CCNaccesserror_visit_sequence(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_sequence(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT) {
        CCNaccesserror_visit_sequence(line, file, func);
    }

    #endif
    return &(node->data.N_visit->visit_children.visit_children_st.sequence);
}

void CCNaccesserror_visit_inputs(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_inputs(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT) {
        CCNaccesserror_visit_inputs(line, file, func);
    }

    #endif
    return &(node->data.N_visit->visit_children.visit_children_st.inputs);
}

void CCNaccesserror_visit_outputs(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_outputs(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT) {
        CCNaccesserror_visit_outputs(line, file, func);
    }

    #endif
    return &(node->data.N_visit->visit_children.visit_children_st.outputs);
}

void CCNaccesserror_visit_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT) {
        CCNaccesserror_visit_next(line, file, func);
    }

    #endif
    return &(node->data.N_visit->visit_children.visit_children_st.next);
}

void CCNaccesserror_visit_inode(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_visit_inode(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT) {
        CCNaccesserror_visit_inode(line, file, func);
    }

    #endif
    return &(node->data.N_visit->inode);
}

void CCNaccesserror_visit_index(int line, const char *file, const char *func);
static inline uint64_t *CCNaccess_visit_index(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT) {
        CCNaccesserror_visit_index(line, file, func);
    }

    #endif
    return &(node->data.N_visit->index);
}

void CCNaccesserror_visit_arg_list_attribute(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_arg_list_attribute(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_ARG_LIST) {
        CCNaccesserror_visit_arg_list_attribute(line, file, func);
    }

    #endif
    return &(node->data.N_visit_arg_list->visit_arg_list_children.visit_arg_list_children_st.attribute);
}

void CCNaccesserror_visit_arg_list_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_visit_arg_list_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_VISIT_ARG_LIST) {
        CCNaccesserror_visit_arg_list_next(line, file, func);
    }

    #endif
    return &(node->data.N_visit_arg_list->visit_arg_list_children.visit_arg_list_children_st.next);
}

void CCNaccesserror_attribute_reference_inode(int line, const char *file, const char *func);
static inline node_st **CCNaccess_attribute_reference_inode(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE_REFERENCE) {
        CCNaccesserror_attribute_reference_inode(line, file, func);
    }

    #endif
    return &(node->data.N_attribute_reference->attribute_reference_children.attribute_reference_children_st.inode);
}

void CCNaccesserror_attribute_reference_iattribute(int line, const char *file, const char *func);
static inline node_st **CCNaccess_attribute_reference_iattribute(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE_REFERENCE) {
        CCNaccesserror_attribute_reference_iattribute(line, file, func);
    }

    #endif
    return &(node->data.N_attribute_reference->attribute_reference_children.attribute_reference_children_st.iattribute);
}

void CCNaccesserror_attribute_reference_node_type(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_attribute_reference_node_type(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE_REFERENCE) {
        CCNaccesserror_attribute_reference_node_type(line, file, func);
    }

    #endif
    return &(node->data.N_attribute_reference->node_type);
}

void CCNaccesserror_attribute_reference_reference(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_attribute_reference_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE_REFERENCE) {
        CCNaccesserror_attribute_reference_reference(line, file, func);
    }

    #endif
    return &(node->data.N_attribute_reference->reference);
}

void CCNaccesserror_attribute_reference_child_reference(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_attribute_reference_child_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE_REFERENCE) {
        CCNaccesserror_attribute_reference_child_reference(line, file, func);
    }

    #endif
    return &(node->data.N_attribute_reference->child_reference);
}

void CCNaccesserror_equation_dependency_iattribute(int line, const char *file, const char *func);
static inline node_st **CCNaccess_equation_dependency_iattribute(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_EQUATION_DEPENDENCY) {
        CCNaccesserror_equation_dependency_iattribute(line, file, func);
    }

    #endif
    return &(node->data.N_equation_dependency->equation_dependency_children.equation_dependency_children_st.iattribute);
}

void CCNaccesserror_equation_dependency_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_equation_dependency_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_EQUATION_DEPENDENCY) {
        CCNaccesserror_equation_dependency_next(line, file, func);
    }

    #endif
    return &(node->data.N_equation_dependency->equation_dependency_children.equation_dependency_children_st.next);
}

void CCNaccesserror_equation_rule(int line, const char *file, const char *func);
static inline node_st **CCNaccess_equation_rule(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_EQUATION) {
        CCNaccesserror_equation_rule(line, file, func);
    }

    #endif
    return &(node->data.N_equation->equation_children.equation_children_st.rule);
}

void CCNaccesserror_equation_iargs(int line, const char *file, const char *func);
static inline node_st **CCNaccess_equation_iargs(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_EQUATION) {
        CCNaccesserror_equation_iargs(line, file, func);
    }

    #endif
    return &(node->data.N_equation->equation_children.equation_children_st.iargs);
}

void CCNaccesserror_equation_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_equation_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_EQUATION) {
        CCNaccesserror_equation_next(line, file, func);
    }

    #endif
    return &(node->data.N_equation->equation_children.equation_children_st.next);
}

void CCNaccesserror_attribute_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_attribute_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_name(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->attribute_children.attribute_children_st.name);
}

void CCNaccesserror_attribute_type_reference(int line, const char *file, const char *func);
static inline node_st **CCNaccess_attribute_type_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_type_reference(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->attribute_children.attribute_children_st.type_reference);
}

void CCNaccesserror_attribute_lifetimes(int line, const char *file, const char *func);
static inline node_st **CCNaccess_attribute_lifetimes(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_lifetimes(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->attribute_children.attribute_children_st.lifetimes);
}

void CCNaccesserror_attribute_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_attribute_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_next(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->attribute_children.attribute_children_st.next);
}

void CCNaccesserror_attribute_type(int line, const char *file, const char *func);
static inline enum attribute_type *CCNaccess_attribute_type(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_type(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->type);
}

void CCNaccesserror_attribute_in_constructor(int line, const char *file, const char *func);
static inline bool *CCNaccess_attribute_in_constructor(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_in_constructor(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->in_constructor);
}

void CCNaccesserror_attribute_is_inherited(int line, const char *file, const char *func);
static inline bool *CCNaccess_attribute_is_inherited(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_is_inherited(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->is_inherited);
}

void CCNaccesserror_attribute_is_synthesized(int line, const char *file, const char *func);
static inline bool *CCNaccess_attribute_is_synthesized(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_is_synthesized(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->is_synthesized);
}

void CCNaccesserror_attribute_is_propagated(int line, const char *file, const char *func);
static inline bool *CCNaccess_attribute_is_propagated(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ATTRIBUTE) {
        CCNaccesserror_attribute_is_propagated(line, file, func);
    }

    #endif
    return &(node->data.N_attribute->is_propagated);
}

void CCNaccesserror_itravdata_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_itravdata_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVDATA) {
        CCNaccesserror_itravdata_name(line, file, func);
    }

    #endif
    return &(node->data.N_itravdata->itravdata_children.itravdata_children_st.name);
}

void CCNaccesserror_itravdata_type_reference(int line, const char *file, const char *func);
static inline node_st **CCNaccess_itravdata_type_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVDATA) {
        CCNaccesserror_itravdata_type_reference(line, file, func);
    }

    #endif
    return &(node->data.N_itravdata->itravdata_children.itravdata_children_st.type_reference);
}

void CCNaccesserror_itravdata_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_itravdata_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVDATA) {
        CCNaccesserror_itravdata_next(line, file, func);
    }

    #endif
    return &(node->data.N_itravdata->itravdata_children.itravdata_children_st.next);
}

void CCNaccesserror_itravdata_type(int line, const char *file, const char *func);
static inline enum attribute_type *CCNaccess_itravdata_type(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVDATA) {
        CCNaccesserror_itravdata_type(line, file, func);
    }

    #endif
    return &(node->data.N_itravdata->type);
}

void CCNaccesserror_itravdata_include_file(int line, const char *file, const char *func);
static inline char * *CCNaccess_itravdata_include_file(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVDATA) {
        CCNaccesserror_itravdata_include_file(line, file, func);
    }

    #endif
    return &(node->data.N_itravdata->include_file);
}

void CCNaccesserror_setoperation_left(int line, const char *file, const char *func);
static inline node_st **CCNaccess_setoperation_left(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_SETOPERATION) {
        CCNaccesserror_setoperation_left(line, file, func);
    }

    #endif
    return &(node->data.N_setoperation->setoperation_children.setoperation_children_st.left);
}

void CCNaccesserror_setoperation_right(int line, const char *file, const char *func);
static inline node_st **CCNaccess_setoperation_right(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_SETOPERATION) {
        CCNaccesserror_setoperation_right(line, file, func);
    }

    #endif
    return &(node->data.N_setoperation->setoperation_children.setoperation_children_st.right);
}

void CCNaccesserror_setoperation_type(int line, const char *file, const char *func);
static inline enum setoperation_type *CCNaccess_setoperation_type(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_SETOPERATION) {
        CCNaccesserror_setoperation_type(line, file, func);
    }

    #endif
    return &(node->data.N_setoperation->type);
}

void CCNaccesserror_setliteral_reference(int line, const char *file, const char *func);
static inline node_st **CCNaccess_setliteral_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_SETLITERAL) {
        CCNaccesserror_setliteral_reference(line, file, func);
    }

    #endif
    return &(node->data.N_setliteral->setliteral_children.setliteral_children_st.reference);
}

void CCNaccesserror_setliteral_left(int line, const char *file, const char *func);
static inline node_st **CCNaccess_setliteral_left(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_SETLITERAL) {
        CCNaccesserror_setliteral_left(line, file, func);
    }

    #endif
    return &(node->data.N_setliteral->setliteral_children.setliteral_children_st.left);
}

void CCNaccesserror_setliteral_right(int line, const char *file, const char *func);
static inline node_st **CCNaccess_setliteral_right(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_SETLITERAL) {
        CCNaccesserror_setliteral_right(line, file, func);
    }

    #endif
    return &(node->data.N_setliteral->setliteral_children.setliteral_children_st.right);
}

void CCNaccesserror_setreference_reference(int line, const char *file, const char *func);
static inline node_st **CCNaccess_setreference_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_SETREFERENCE) {
        CCNaccesserror_setreference_reference(line, file, func);
    }

    #endif
    return &(node->data.N_setreference->setreference_children.setreference_children_st.reference);
}

void CCNaccesserror_ste_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ste_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_STE) {
        CCNaccesserror_ste_next(line, file, func);
    }

    #endif
    return &(node->data.N_ste->ste_children.ste_children_st.next);
}

void CCNaccesserror_ste_key(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_ste_key(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_STE) {
        CCNaccesserror_ste_key(line, file, func);
    }

    #endif
    return &(node->data.N_ste->key);
}

void CCNaccesserror_ste_value(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_ste_value(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_STE) {
        CCNaccesserror_ste_value(line, file, func);
    }

    #endif
    return &(node->data.N_ste->value);
}

void CCNaccesserror_child_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_child_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_CHILD) {
        CCNaccesserror_child_name(line, file, func);
    }

    #endif
    return &(node->data.N_child->child_children.child_children_st.name);
}

void CCNaccesserror_child_lifetimes(int line, const char *file, const char *func);
static inline node_st **CCNaccess_child_lifetimes(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_CHILD) {
        CCNaccesserror_child_lifetimes(line, file, func);
    }

    #endif
    return &(node->data.N_child->child_children.child_children_st.lifetimes);
}

void CCNaccesserror_child_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_child_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_CHILD) {
        CCNaccesserror_child_next(line, file, func);
    }

    #endif
    return &(node->data.N_child->child_children.child_children_st.next);
}

void CCNaccesserror_child_type_reference(int line, const char *file, const char *func);
static inline node_st **CCNaccess_child_type_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_CHILD) {
        CCNaccesserror_child_type_reference(line, file, func);
    }

    #endif
    return &(node->data.N_child->child_children.child_children_st.type_reference);
}

void CCNaccesserror_child_type(int line, const char *file, const char *func);
static inline enum child_type *CCNaccess_child_type(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_CHILD) {
        CCNaccesserror_child_type(line, file, func);
    }

    #endif
    return &(node->data.N_child->type);
}

void CCNaccesserror_child_in_constructor(int line, const char *file, const char *func);
static inline bool *CCNaccess_child_in_constructor(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_CHILD) {
        CCNaccesserror_child_in_constructor(line, file, func);
    }

    #endif
    return &(node->data.N_child->in_constructor);
}

void CCNaccesserror_child_is_mandatory(int line, const char *file, const char *func);
static inline bool *CCNaccess_child_is_mandatory(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_CHILD) {
        CCNaccesserror_child_is_mandatory(line, file, func);
    }

    #endif
    return &(node->data.N_child->is_mandatory);
}

void CCNaccesserror_lifetime_range_target(int line, const char *file, const char *func);
static inline node_st **CCNaccess_lifetime_range_target(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_LIFETIME_RANGE) {
        CCNaccesserror_lifetime_range_target(line, file, func);
    }

    #endif
    return &(node->data.N_lifetime_range->lifetime_range_children.lifetime_range_children_st.target);
}

void CCNaccesserror_lifetime_range_inclusive(int line, const char *file, const char *func);
static inline bool *CCNaccess_lifetime_range_inclusive(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_LIFETIME_RANGE) {
        CCNaccesserror_lifetime_range_inclusive(line, file, func);
    }

    #endif
    return &(node->data.N_lifetime_range->inclusive);
}

void CCNaccesserror_lifetime_range_action_id(int line, const char *file, const char *func);
static inline int *CCNaccess_lifetime_range_action_id(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_LIFETIME_RANGE) {
        CCNaccesserror_lifetime_range_action_id(line, file, func);
    }

    #endif
    return &(node->data.N_lifetime_range->action_id);
}

void CCNaccesserror_lifetime_range_next_action_id(int line, const char *file, const char *func);
static inline int *CCNaccess_lifetime_range_next_action_id(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_LIFETIME_RANGE) {
        CCNaccesserror_lifetime_range_next_action_id(line, file, func);
    }

    #endif
    return &(node->data.N_lifetime_range->next_action_id);
}

void CCNaccesserror_ilifetime_begin(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ilifetime_begin(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ILIFETIME) {
        CCNaccesserror_ilifetime_begin(line, file, func);
    }

    #endif
    return &(node->data.N_ilifetime->ilifetime_children.ilifetime_children_st.begin);
}

void CCNaccesserror_ilifetime_end(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ilifetime_end(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ILIFETIME) {
        CCNaccesserror_ilifetime_end(line, file, func);
    }

    #endif
    return &(node->data.N_ilifetime->ilifetime_children.ilifetime_children_st.end);
}

void CCNaccesserror_ilifetime_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ilifetime_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ILIFETIME) {
        CCNaccesserror_ilifetime_next(line, file, func);
    }

    #endif
    return &(node->data.N_ilifetime->ilifetime_children.ilifetime_children_st.next);
}

void CCNaccesserror_ilifetime_type(int line, const char *file, const char *func);
static inline enum lifetime_type *CCNaccess_ilifetime_type(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ILIFETIME) {
        CCNaccesserror_ilifetime_type(line, file, func);
    }

    #endif
    return &(node->data.N_ilifetime->type);
}

void CCNaccesserror_nodeset_child_entry_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_nodeset_child_entry_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_NODESET_CHILD_ENTRY) {
        CCNaccesserror_nodeset_child_entry_next(line, file, func);
    }

    #endif
    return &(node->data.N_nodeset_child_entry->nodeset_child_entry_children.nodeset_child_entry_children_st.next);
}

void CCNaccesserror_nodeset_child_entry_reference(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_nodeset_child_entry_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_NODESET_CHILD_ENTRY) {
        CCNaccesserror_nodeset_child_entry_reference(line, file, func);
    }

    #endif
    return &(node->data.N_nodeset_child_entry->reference);
}

void CCNaccesserror_inodeset_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inodeset_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODESET) {
        CCNaccesserror_inodeset_name(line, file, func);
    }

    #endif
    return &(node->data.N_inodeset->inodeset_children.inodeset_children_st.name);
}

void CCNaccesserror_inodeset_expr(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inodeset_expr(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODESET) {
        CCNaccesserror_inodeset_expr(line, file, func);
    }

    #endif
    return &(node->data.N_inodeset->inodeset_children.inodeset_children_st.expr);
}

void CCNaccesserror_inodeset_iattributes(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inodeset_iattributes(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODESET) {
        CCNaccesserror_inodeset_iattributes(line, file, func);
    }

    #endif
    return &(node->data.N_inodeset->inodeset_children.inodeset_children_st.iattributes);
}

void CCNaccesserror_inodeset_unpacked(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inodeset_unpacked(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODESET) {
        CCNaccesserror_inodeset_unpacked(line, file, func);
    }

    #endif
    return &(node->data.N_inodeset->inodeset_children.inodeset_children_st.unpacked);
}

void CCNaccesserror_inodeset_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inodeset_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODESET) {
        CCNaccesserror_inodeset_next(line, file, func);
    }

    #endif
    return &(node->data.N_inodeset->inodeset_children.inodeset_children_st.next);
}

void CCNaccesserror_inodeset_children_table(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inodeset_children_table(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODESET) {
        CCNaccesserror_inodeset_children_table(line, file, func);
    }

    #endif
    return &(node->data.N_inodeset->inodeset_children.inodeset_children_st.children_table);
}

void CCNaccesserror_inodeset_iinfo(int line, const char *file, const char *func);
static inline char * *CCNaccess_inodeset_iinfo(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODESET) {
        CCNaccesserror_inodeset_iinfo(line, file, func);
    }

    #endif
    return &(node->data.N_inodeset->iinfo);
}

void CCNaccesserror_inodeset_illegal_setexpr_attr(int line, const char *file, const char *func);
static inline bool *CCNaccess_inodeset_illegal_setexpr_attr(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODESET) {
        CCNaccesserror_inodeset_illegal_setexpr_attr(line, file, func);
    }

    #endif
    return &(node->data.N_inodeset->illegal_setexpr_attr);
}

void CCNaccesserror_inode_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inode_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_name(line, file, func);
    }

    #endif
    return &(node->data.N_inode->inode_children.inode_children_st.name);
}

void CCNaccesserror_inode_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inode_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_next(line, file, func);
    }

    #endif
    return &(node->data.N_inode->inode_children.inode_children_st.next);
}

void CCNaccesserror_inode_ichildren(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inode_ichildren(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_ichildren(line, file, func);
    }

    #endif
    return &(node->data.N_inode->inode_children.inode_children_st.ichildren);
}

void CCNaccesserror_inode_iattributes(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inode_iattributes(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_iattributes(line, file, func);
    }

    #endif
    return &(node->data.N_inode->inode_children.inode_children_st.iattributes);
}

void CCNaccesserror_inode_iequations(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inode_iequations(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_iequations(line, file, func);
    }

    #endif
    return &(node->data.N_inode->inode_children.inode_children_st.iequations);
}

void CCNaccesserror_inode_lifetimes(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inode_lifetimes(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_lifetimes(line, file, func);
    }

    #endif
    return &(node->data.N_inode->inode_children.inode_children_st.lifetimes);
}

void CCNaccesserror_inode_visit(int line, const char *file, const char *func);
static inline node_st **CCNaccess_inode_visit(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_visit(line, file, func);
    }

    #endif
    return &(node->data.N_inode->inode_children.inode_children_st.visit);
}

void CCNaccesserror_inode_iifno(int line, const char *file, const char *func);
static inline char * *CCNaccess_inode_iifno(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_iifno(line, file, func);
    }

    #endif
    return &(node->data.N_inode->iifno);
}

void CCNaccesserror_inode_is_root(int line, const char *file, const char *func);
static inline bool *CCNaccess_inode_is_root(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_is_root(line, file, func);
    }

    #endif
    return &(node->data.N_inode->is_root);
}

void CCNaccesserror_inode_index(int line, const char *file, const char *func);
static inline int *CCNaccess_inode_index(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_INODE) {
        CCNaccesserror_inode_index(line, file, func);
    }

    #endif
    return &(node->data.N_inode->index);
}

void CCNaccesserror_ipass_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ipass_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPASS) {
        CCNaccesserror_ipass_name(line, file, func);
    }

    #endif
    return &(node->data.N_ipass->ipass_children.ipass_children_st.name);
}

void CCNaccesserror_ipass_iprefix(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ipass_iprefix(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPASS) {
        CCNaccesserror_ipass_iprefix(line, file, func);
    }

    #endif
    return &(node->data.N_ipass->ipass_children.ipass_children_st.iprefix);
}

void CCNaccesserror_ipass_target_func(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ipass_target_func(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPASS) {
        CCNaccesserror_ipass_target_func(line, file, func);
    }

    #endif
    return &(node->data.N_ipass->ipass_children.ipass_children_st.target_func);
}

void CCNaccesserror_ipass_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ipass_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPASS) {
        CCNaccesserror_ipass_next(line, file, func);
    }

    #endif
    return &(node->data.N_ipass->ipass_children.ipass_children_st.next);
}

void CCNaccesserror_ipass_iifno(int line, const char *file, const char *func);
static inline char * *CCNaccess_ipass_iifno(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPASS) {
        CCNaccesserror_ipass_iifno(line, file, func);
    }

    #endif
    return &(node->data.N_ipass->iifno);
}

void CCNaccesserror_itraversal_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_itraversal_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVERSAL) {
        CCNaccesserror_itraversal_name(line, file, func);
    }

    #endif
    return &(node->data.N_itraversal->itraversal_children.itraversal_children_st.name);
}

void CCNaccesserror_itraversal_iprefix(int line, const char *file, const char *func);
static inline node_st **CCNaccess_itraversal_iprefix(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVERSAL) {
        CCNaccesserror_itraversal_iprefix(line, file, func);
    }

    #endif
    return &(node->data.N_itraversal->itraversal_children.itraversal_children_st.iprefix);
}

void CCNaccesserror_itraversal_inodes(int line, const char *file, const char *func);
static inline node_st **CCNaccess_itraversal_inodes(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVERSAL) {
        CCNaccesserror_itraversal_inodes(line, file, func);
    }

    #endif
    return &(node->data.N_itraversal->itraversal_children.itraversal_children_st.inodes);
}

void CCNaccesserror_itraversal_data(int line, const char *file, const char *func);
static inline node_st **CCNaccess_itraversal_data(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVERSAL) {
        CCNaccesserror_itraversal_data(line, file, func);
    }

    #endif
    return &(node->data.N_itraversal->itraversal_children.itraversal_children_st.data);
}

void CCNaccesserror_itraversal_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_itraversal_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVERSAL) {
        CCNaccesserror_itraversal_next(line, file, func);
    }

    #endif
    return &(node->data.N_itraversal->itraversal_children.itraversal_children_st.next);
}

void CCNaccesserror_itraversal_index(int line, const char *file, const char *func);
static inline int *CCNaccess_itraversal_index(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVERSAL) {
        CCNaccesserror_itraversal_index(line, file, func);
    }

    #endif
    return &(node->data.N_itraversal->index);
}

void CCNaccesserror_itraversal_iinfo(int line, const char *file, const char *func);
static inline char * *CCNaccess_itraversal_iinfo(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_ITRAVERSAL) {
        CCNaccesserror_itraversal_iinfo(line, file, func);
    }

    #endif
    return &(node->data.N_itraversal->iinfo);
}

void CCNaccesserror_iphase_name(int line, const char *file, const char *func);
static inline node_st **CCNaccess_iphase_name(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPHASE) {
        CCNaccesserror_iphase_name(line, file, func);
    }

    #endif
    return &(node->data.N_iphase->iphase_children.iphase_children_st.name);
}

void CCNaccesserror_iphase_iprefix(int line, const char *file, const char *func);
static inline node_st **CCNaccess_iphase_iprefix(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPHASE) {
        CCNaccesserror_iphase_iprefix(line, file, func);
    }

    #endif
    return &(node->data.N_iphase->iphase_children.iphase_children_st.iprefix);
}

void CCNaccesserror_iphase_gate_func(int line, const char *file, const char *func);
static inline node_st **CCNaccess_iphase_gate_func(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPHASE) {
        CCNaccesserror_iphase_gate_func(line, file, func);
    }

    #endif
    return &(node->data.N_iphase->iphase_children.iphase_children_st.gate_func);
}

void CCNaccesserror_iphase_iactions(int line, const char *file, const char *func);
static inline node_st **CCNaccess_iphase_iactions(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPHASE) {
        CCNaccesserror_iphase_iactions(line, file, func);
    }

    #endif
    return &(node->data.N_iphase->iphase_children.iphase_children_st.iactions);
}

void CCNaccesserror_iphase_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_iphase_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPHASE) {
        CCNaccesserror_iphase_next(line, file, func);
    }

    #endif
    return &(node->data.N_iphase->iphase_children.iphase_children_st.next);
}

void CCNaccesserror_iphase_is_start(int line, const char *file, const char *func);
static inline bool *CCNaccess_iphase_is_start(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPHASE) {
        CCNaccesserror_iphase_is_start(line, file, func);
    }

    #endif
    return &(node->data.N_iphase->is_start);
}

void CCNaccesserror_iphase_is_cycle(int line, const char *file, const char *func);
static inline bool *CCNaccess_iphase_is_cycle(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPHASE) {
        CCNaccesserror_iphase_is_cycle(line, file, func);
    }

    #endif
    return &(node->data.N_iphase->is_cycle);
}

void CCNaccesserror_iphase_iinfo(int line, const char *file, const char *func);
static inline char * *CCNaccess_iphase_iinfo(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IPHASE) {
        CCNaccesserror_iphase_iinfo(line, file, func);
    }

    #endif
    return &(node->data.N_iphase->iinfo);
}

void CCNaccesserror_iactions_reference(int line, const char *file, const char *func);
static inline node_st **CCNaccess_iactions_reference(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IACTIONS) {
        CCNaccesserror_iactions_reference(line, file, func);
    }

    #endif
    return &(node->data.N_iactions->iactions_children.iactions_children_st.reference);
}

void CCNaccesserror_iactions_next(int line, const char *file, const char *func);
static inline node_st **CCNaccess_iactions_next(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IACTIONS) {
        CCNaccesserror_iactions_next(line, file, func);
    }

    #endif
    return &(node->data.N_iactions->iactions_children.iactions_children_st.next);
}

void CCNaccesserror_iactions_action_id(int line, const char *file, const char *func);
static inline int *CCNaccess_iactions_action_id(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_IACTIONS) {
        CCNaccesserror_iactions_action_id(line, file, func);
    }

    #endif
    return &(node->data.N_iactions->action_id);
}

void CCNaccesserror_ast_iphases(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ast_iphases(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_iphases(line, file, func);
    }

    #endif
    return &(node->data.N_ast->ast_children.ast_children_st.iphases);
}

void CCNaccesserror_ast_itraversals(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ast_itraversals(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_itraversals(line, file, func);
    }

    #endif
    return &(node->data.N_ast->ast_children.ast_children_st.itraversals);
}

void CCNaccesserror_ast_ipasses(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ast_ipasses(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_ipasses(line, file, func);
    }

    #endif
    return &(node->data.N_ast->ast_children.ast_children_st.ipasses);
}

void CCNaccesserror_ast_inodes(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ast_inodes(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_inodes(line, file, func);
    }

    #endif
    return &(node->data.N_ast->ast_children.ast_children_st.inodes);
}

void CCNaccesserror_ast_inodesets(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ast_inodesets(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_inodesets(line, file, func);
    }

    #endif
    return &(node->data.N_ast->ast_children.ast_children_st.inodesets);
}

void CCNaccesserror_ast_enums(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ast_enums(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_enums(line, file, func);
    }

    #endif
    return &(node->data.N_ast->ast_children.ast_children_st.enums);
}

void CCNaccesserror_ast_stable(int line, const char *file, const char *func);
static inline node_st **CCNaccess_ast_stable(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_stable(line, file, func);
    }

    #endif
    return &(node->data.N_ast->ast_children.ast_children_st.stable);
}

void CCNaccesserror_ast_num_traversals(int line, const char *file, const char *func);
static inline int *CCNaccess_ast_num_traversals(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_num_traversals(line, file, func);
    }

    #endif
    return &(node->data.N_ast->num_traversals);
}

void CCNaccesserror_ast_num_nodes(int line, const char *file, const char *func);
static inline int *CCNaccess_ast_num_nodes(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_num_nodes(line, file, func);
    }

    #endif
    return &(node->data.N_ast->num_nodes);
}

void CCNaccesserror_ast_root_node(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_ast_root_node(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_root_node(line, file, func);
    }

    #endif
    return &(node->data.N_ast->root_node);
}

void CCNaccesserror_ast_start_phase(int line, const char *file, const char *func);
static inline node_st * *CCNaccess_ast_start_phase(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_start_phase(line, file, func);
    }

    #endif
    return &(node->data.N_ast->start_phase);
}

void CCNaccesserror_ast_uses_unsafe(int line, const char *file, const char *func);
static inline bool *CCNaccess_ast_uses_unsafe(int line, const char *file, const char *func, node_st *node) {
    #ifndef NDEBUG
    if (NODE_TYPE(node) != NT_AST) {
        CCNaccesserror_ast_uses_unsafe(line, file, func);
    }

    #endif
    return &(node->data.N_ast->uses_unsafe);
}

#ifdef NDEBUG
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#endif

