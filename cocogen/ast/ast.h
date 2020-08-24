#pragma once

#include <stdlib.h>
#include <sys/queue.h>
#include <stdbool.h>

#include "palm/set.h"
#include "palm/hash_table.h"

enum symbol_table_entry_type {
    STE_PHASE, STE_TRAVERSAL, STE_PASS, STE_ENUM, STE_NODE, STE_NODESET
};

struct symbol_table_entry {
    enum symbol_table_entry_type type;
    void *data;
};

/**
 * @brief The possible operations to perform on two set expressions.
 */
enum ccn_set_operator { CCN_SET_UNION, CCN_SET_INTERSECT, CCN_SET_DIFFERENCE };
enum ccn_set_type { CCN_SET_REFERENCE, CCN_SET_LITERAL, CCN_SET_OPERATION };


enum action_type {
    ACTION_PHASE, ACTION_PASS, ACTION_TRAVERSAL, ACTION_REFERENCE
};

enum attribute_type {
    AT_int,
    AT_uint,
    AT_int8,
    AT_int16,
    AT_int32,
    AT_int64,
    AT_uint8,
    AT_uint16,
    AT_uint32,
    AT_uint64,

    AT_float,
    AT_double,

    AT_bool,
    AT_string,
    AT_link_or_enum,
    AT_link,
    AT_enum,
};

enum attribute_value_type {
    AV_string,
    AV_int,
    AV_uint,
    AV_float,
    AV_double,
    AV_bool,
    AV_id
};

struct node_wrapper {
    struct node *node;
    SLIST_ENTRY(node_wrapper) next;
};

struct hash_info {
    // contains the unique identifier for all data needed in codegen.
    char *hash;
    bool hash_matches;
};

struct id {
    char *orig;
    char *lwr;
    char *upr;

    SLIST_ENTRY(id) next;

    struct ctinfo *loc_info;
};

SLIST_HEAD(id_slist, id);

struct attribute {
    struct id *name;
    struct id *type_reference; // The type can be primitive or a link to an enum/node(this)

    enum attribute_type type;

    char *include_file; // Optional include file for trav data.
    bool in_constructor; // Denotes if this attribute should be in the constructor for the node it belongs to.

    SLIST_ENTRY(attribute) next;

    struct ctinfo *loc_info;
};

enum child_type {
    CT_NODE, CT_NODESET, CT_NULL
};

struct child {
    struct id *name;
    struct id *type_reference;

    enum child_type type;
    union { // A child is either of type node or nodeset.
        struct node *node;
        struct nodeset *nodeset;
    };

    bool is_mandatory;
    bool in_constructor;

    SLIST_ENTRY(child) next;

    struct ctinfo *loc_info;
};

/**
 * @struct ast_set_operation
 * @brief The structure to store a set operation on two expressions.
 *
 * @details A set operation is an operation, like a UNION or INTERSECT
 *  on two set expressions.
 */
struct ccn_set_operation {
    enum ccn_set_operator operator;
    struct ccn_set_expr *left_child;
    struct ccn_set_expr *right_child;
    struct ctinfo *loc_info;
};

/**
 * @struct ccn_set_expr
 * @brief The structure to store an internal set expression.
 *
 * @details An internal set expression can be of three possible types:
 *   -# reference:
 *        Reference to a defined nodeset.
 *   -# set:
 *        A set of node ids (:: struct id), which is defined inline, so by placing between
 *        {} brackets.
 *   -# operation:
 *        A set operation on two node sets. This node set will be the result
 *        of that operation.
 *
 * We use this, because users can denote nodes as a set expressions
 * and perform operations with the sets.
 *
 * For example, a user can define the following:
 *   nodeset Expr = {Num, BinOp} | Monop
 * ,where the | Monop denotes a UNION with the Monop set.
 *
 */
struct ccn_set_expr {
    enum ccn_set_type type;

    union {
        struct id *reference;
        set_st *set;
        struct ccn_set_operation *operation;
    };

    struct ctinfo *loc_info;
};


struct ccn_enum {
    struct id *name;
    struct id *prefix;
    char *info;

    struct id_slist *values;

    STAILQ_ENTRY(ccn_enum) next;

    struct ctinfo *loc_info;
};


struct pass {
    struct id *name;
    struct id *prefix;
    char *info;

    struct id *target_func;

    STAILQ_ENTRY(pass) next;
    struct ctinfo *loc_info;

};

struct traversal {
    struct id *name;
    struct id *prefix;
    char *info;

    union {
        struct ccn_set_expr *target_nodes_expr;
        struct node_wrapper_slist *nodes;
    };

    STAILQ_ENTRY(traversal) next;
    struct ctinfo *loc_info;

};

SLIST_HEAD(node_wrapper_slist, node_wrapper);
SLIST_HEAD(child_slist, child);
SLIST_HEAD(attribute_slist, attribute);
SLIST_HEAD(action_slist, action);
STAILQ_HEAD(phase_stailq, phase);
STAILQ_HEAD(pass_stailq, pass);
STAILQ_HEAD(traversal_stailq, traversal);
STAILQ_HEAD(node_stailq, node);
STAILQ_HEAD(nodeset_stailq, nodeset);
STAILQ_HEAD(ccn_enum_stailq, ccn_enum);

struct node {
    struct id *name;
    char *info;
    bool is_root;

    struct attribute_slist *attributes;
    struct child_slist *children;

    STAILQ_ENTRY(node) next;
    struct ctinfo *loc_info;
};

enum nodeset_type {
    NS_SETEXPR, NS_LISTOFNODES
};

struct nodeset {
    struct id *name;
    char *info;

    enum nodeset_type type;
    union {
        struct ccn_set_expr *expr;
        struct node_wrapper_slist *nodes;
    };

    STAILQ_ENTRY(nodeset) next;
    struct ctinfo *loc_info;
};


struct phase {
    struct id *name;
    struct id *prefix;
    char *info;

    struct id *gate_func;

    bool is_cycle;
    bool is_start;

    STAILQ_ENTRY(phase) next;

    struct action_slist *actions;
    struct ctinfo *loc_info;

};

struct action {
    enum action_type action_type;
    union {
        struct phase *phase;
        struct traversal *traversal;
        struct pass *pass;
        char *reference_to_action;
    };

    SLIST_ENTRY(action) next;
    struct ctinfo *loc_info;

};

struct ast {
    struct phase_stailq *phases;
    struct traversal_stailq *traversals;
    struct pass_stailq *passes;
    struct node_stailq *nodes;
    struct nodeset_stailq *nodesets;
    struct ccn_enum_stailq *enums;

    struct node *root_node;
    struct phase *start_phase;

    htable_st *symbol_table;
};


/* Helper functions */

size_t ASTnumOfChildren(struct node *node);
size_t ASTnumOfTraversals(struct ast *ast);
size_t ASTnumOfNodes(struct ast *ast);
char *ASTattributeTypeToString(struct attribute *attr);
char *ASTattributeDefaultValue(struct attribute *attribute);
set_st *ASTnewSet_Id(size_t size);
