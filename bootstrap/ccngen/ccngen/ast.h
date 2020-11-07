#pragma once
#pragma once
#include "ccn/ccn_types.h"
#include "ccngen/enum.h"
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

struct NODE_DATA_ATTRIBUTE {
    union NODE_CHILDREN_ATTRIBUTE {
        struct NODE_CHILDREN_ATTRIBUTE_STRUCT {
            node_st *name;
            node_st *type_reference;
            node_st *next;
        } attribute_children_st;

        node_st *attribute_children_at[3];
    } attribute_children;

    enum attribute_type type;
    int in_constructor;
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
            node_st *next;
        } setliteral_children_st;

        node_st *setliteral_children_at[2];
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
            node_st *type_reference;
            node_st *next;
        } child_children_st;

        node_st *child_children_at[3];
    } child_children;

    enum child_type type;
    int in_constructor;
    int is_mandatory;
};

struct NODE_DATA_INODESET {
    union NODE_CHILDREN_INODESET {
        struct NODE_CHILDREN_INODESET_STRUCT {
            node_st *name;
            node_st *expr;
            node_st *unpacked;
            node_st *next;
        } inodeset_children_st;

        node_st *inodeset_children_at[4];
    } inodeset_children;

    char * iinfo;
};

struct NODE_DATA_INODE {
    union NODE_CHILDREN_INODE {
        struct NODE_CHILDREN_INODE_STRUCT {
            node_st *name;
            node_st *next;
            node_st *ichildren;
            node_st *iattributes;
        } inode_children_st;

        node_st *inode_children_at[4];
    } inode_children;

    char * iifno;
    int is_root;
    int index;
};

struct NODE_DATA_IPASS {
    union NODE_CHILDREN_IPASS {
        struct NODE_CHILDREN_IPASS_STRUCT {
            node_st *next;
        } ipass_children_st;

        node_st *ipass_children_at[1];
    } ipass_children;

    node_st *name;
    char * iifno;
    node_st *iprefix;
    node_st *target_func;
};

struct NODE_DATA_ITRAVERSAL {
    union NODE_CHILDREN_ITRAVERSAL {
        struct NODE_CHILDREN_ITRAVERSAL_STRUCT {
            node_st *inodes;
            node_st *data;
            node_st *next;
        } itraversal_children_st;

        node_st *itraversal_children_at[3];
    } itraversal_children;

    int index;
    node_st *name;
    char * iinfo;
    node_st *iprefix;
};

struct NODE_DATA_IPHASE {
    union NODE_CHILDREN_IPHASE {
        struct NODE_CHILDREN_IPHASE_STRUCT {
            node_st *iactions;
            node_st *next;
        } iphase_children_st;

        node_st *iphase_children_at[2];
    } iphase_children;

    node_st *name;
    int is_start;
    int is_cycle;
    char * iinfo;
    node_st *iprefix;
    node_st *gate_func;
};

struct NODE_DATA_IACTIONS {
    union NODE_CHILDREN_IACTIONS {
        struct NODE_CHILDREN_IACTIONS_STRUCT {
            node_st *next;
        } iactions_children_st;

        node_st *iactions_children_at[1];
    } iactions_children;

    node_st *reference;
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
};

#define ID_NEXT(n) ((n)->data.N_id->id_children.id_children_st.next)
#define ID_ORIG(n) ((n)->data.N_id->orig)
#define ID_LWR(n) ((n)->data.N_id->lwr)
#define ID_UPR(n) ((n)->data.N_id->upr)
#define IENUM_VALS(n) ((n)->data.N_ienum->ienum_children.ienum_children_st.vals)
#define IENUM_NAME(n) ((n)->data.N_ienum->ienum_children.ienum_children_st.name)
#define IENUM_IPREFIX(n) ((n)->data.N_ienum->ienum_children.ienum_children_st.iprefix)
#define IENUM_NEXT(n) ((n)->data.N_ienum->ienum_children.ienum_children_st.next)
#define IENUM_IINFO(n) ((n)->data.N_ienum->iinfo)
#define ATTRIBUTE_NAME(n) ((n)->data.N_attribute->attribute_children.attribute_children_st.name)
#define ATTRIBUTE_TYPE_REFERENCE(n) ((n)->data.N_attribute->attribute_children.attribute_children_st.type_reference)
#define ATTRIBUTE_NEXT(n) ((n)->data.N_attribute->attribute_children.attribute_children_st.next)
#define ATTRIBUTE_TYPE(n) ((n)->data.N_attribute->type)
#define ATTRIBUTE_IN_CONSTRUCTOR(n) ((n)->data.N_attribute->in_constructor)
#define ITRAVDATA_NAME(n) ((n)->data.N_itravdata->itravdata_children.itravdata_children_st.name)
#define ITRAVDATA_TYPE_REFERENCE(n) ((n)->data.N_itravdata->itravdata_children.itravdata_children_st.type_reference)
#define ITRAVDATA_NEXT(n) ((n)->data.N_itravdata->itravdata_children.itravdata_children_st.next)
#define ITRAVDATA_TYPE(n) ((n)->data.N_itravdata->type)
#define ITRAVDATA_INCLUDE_FILE(n) ((n)->data.N_itravdata->include_file)
#define SETOPERATION_LEFT(n) ((n)->data.N_setoperation->setoperation_children.setoperation_children_st.left)
#define SETOPERATION_RIGHT(n) ((n)->data.N_setoperation->setoperation_children.setoperation_children_st.right)
#define SETOPERATION_TYPE(n) ((n)->data.N_setoperation->type)
#define SETLITERAL_REFERENCE(n) ((n)->data.N_setliteral->setliteral_children.setliteral_children_st.reference)
#define SETLITERAL_NEXT(n) ((n)->data.N_setliteral->setliteral_children.setliteral_children_st.next)
#define SETREFERENCE_REFERENCE(n) ((n)->data.N_setreference->setreference_children.setreference_children_st.reference)
#define STE_NEXT(n) ((n)->data.N_ste->ste_children.ste_children_st.next)
#define STE_KEY(n) ((n)->data.N_ste->key)
#define STE_VALUE(n) ((n)->data.N_ste->value)
#define CHILD_NAME(n) ((n)->data.N_child->child_children.child_children_st.name)
#define CHILD_TYPE_REFERENCE(n) ((n)->data.N_child->child_children.child_children_st.type_reference)
#define CHILD_NEXT(n) ((n)->data.N_child->child_children.child_children_st.next)
#define CHILD_TYPE(n) ((n)->data.N_child->type)
#define CHILD_IN_CONSTRUCTOR(n) ((n)->data.N_child->in_constructor)
#define CHILD_IS_MANDATORY(n) ((n)->data.N_child->is_mandatory)
#define INODESET_NAME(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.name)
#define INODESET_EXPR(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.expr)
#define INODESET_UNPACKED(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.unpacked)
#define INODESET_NEXT(n) ((n)->data.N_inodeset->inodeset_children.inodeset_children_st.next)
#define INODESET_IINFO(n) ((n)->data.N_inodeset->iinfo)
#define INODE_NAME(n) ((n)->data.N_inode->inode_children.inode_children_st.name)
#define INODE_NEXT(n) ((n)->data.N_inode->inode_children.inode_children_st.next)
#define INODE_ICHILDREN(n) ((n)->data.N_inode->inode_children.inode_children_st.ichildren)
#define INODE_IATTRIBUTES(n) ((n)->data.N_inode->inode_children.inode_children_st.iattributes)
#define INODE_IIFNO(n) ((n)->data.N_inode->iifno)
#define INODE_IS_ROOT(n) ((n)->data.N_inode->is_root)
#define INODE_INDEX(n) ((n)->data.N_inode->index)
#define IPASS_NEXT(n) ((n)->data.N_ipass->ipass_children.ipass_children_st.next)
#define IPASS_NAME(n) ((n)->data.N_ipass->name)
#define IPASS_IIFNO(n) ((n)->data.N_ipass->iifno)
#define IPASS_IPREFIX(n) ((n)->data.N_ipass->iprefix)
#define IPASS_TARGET_FUNC(n) ((n)->data.N_ipass->target_func)
#define ITRAVERSAL_INODES(n) ((n)->data.N_itraversal->itraversal_children.itraversal_children_st.inodes)
#define ITRAVERSAL_DATA(n) ((n)->data.N_itraversal->itraversal_children.itraversal_children_st.data)
#define ITRAVERSAL_NEXT(n) ((n)->data.N_itraversal->itraversal_children.itraversal_children_st.next)
#define ITRAVERSAL_INDEX(n) ((n)->data.N_itraversal->index)
#define ITRAVERSAL_NAME(n) ((n)->data.N_itraversal->name)
#define ITRAVERSAL_IINFO(n) ((n)->data.N_itraversal->iinfo)
#define ITRAVERSAL_IPREFIX(n) ((n)->data.N_itraversal->iprefix)
#define IPHASE_IACTIONS(n) ((n)->data.N_iphase->iphase_children.iphase_children_st.iactions)
#define IPHASE_NEXT(n) ((n)->data.N_iphase->iphase_children.iphase_children_st.next)
#define IPHASE_NAME(n) ((n)->data.N_iphase->name)
#define IPHASE_IS_START(n) ((n)->data.N_iphase->is_start)
#define IPHASE_IS_CYCLE(n) ((n)->data.N_iphase->is_cycle)
#define IPHASE_IINFO(n) ((n)->data.N_iphase->iinfo)
#define IPHASE_IPREFIX(n) ((n)->data.N_iphase->iprefix)
#define IPHASE_GATE_FUNC(n) ((n)->data.N_iphase->gate_func)
#define IACTIONS_NEXT(n) ((n)->data.N_iactions->iactions_children.iactions_children_st.next)
#define IACTIONS_REFERENCE(n) ((n)->data.N_iactions->reference)
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
node_st *ASTid(char * orig, char * lwr, char * upr);
node_st *ASTienum(node_st *vals, node_st *name, node_st *iprefix, char * iinfo);
node_st *ASTattribute();
node_st *ASTitravdata(node_st *name);
node_st *ASTsetoperation(node_st *left, node_st *right, enum setoperation_type type);
node_st *ASTsetliteral();
node_st *ASTsetreference();
node_st *ASTste();
node_st *ASTchild(node_st *name);
node_st *ASTinodeset();
node_st *ASTinode(node_st *name, char * iifno);
node_st *ASTipass(node_st *name, char * iifno, node_st *iprefix);
node_st *ASTitraversal(node_st *name);
node_st *ASTiphase(node_st *name, int is_start);
node_st *ASTiactions();
node_st *ASTast();
union NODE_DATA {
    struct NODE_DATA_AST *N_ast;
    struct NODE_DATA_IACTIONS *N_iactions;
    struct NODE_DATA_IPHASE *N_iphase;
    struct NODE_DATA_ITRAVERSAL *N_itraversal;
    struct NODE_DATA_IPASS *N_ipass;
    struct NODE_DATA_INODE *N_inode;
    struct NODE_DATA_INODESET *N_inodeset;
    struct NODE_DATA_CHILD *N_child;
    struct NODE_DATA_STE *N_ste;
    struct NODE_DATA_SETREFERENCE *N_setreference;
    struct NODE_DATA_SETLITERAL *N_setliteral;
    struct NODE_DATA_SETOPERATION *N_setoperation;
    struct NODE_DATA_ITRAVDATA *N_itravdata;
    struct NODE_DATA_ATTRIBUTE *N_attribute;
    struct NODE_DATA_IENUM *N_ienum;
    struct NODE_DATA_ID *N_id;
};

#define NODE_TYPE(n) ((n)->nodetype)
#define NODE_CHILDREN(n) ((n)->children)
#define NODE_NUMCHILDREN(n) ((n)->num_children)
struct ccn_node {
    enum ccn_nodetype nodetype;
    union NODE_DATA data;
    struct ccn_node **children;
    long int num_children;
};

