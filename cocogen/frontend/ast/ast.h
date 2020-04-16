#pragma once

/**
 * @file include/cocogen/ast.h
 * @brief The definition of all the types in the internal AST.
 */

#include <stdbool.h>
#include <stdint.h>

#include "lib/array.h"
#include "lib/set.h"

typedef struct SetExpr SetExpr;

typedef struct NodeCommonInfo {
    // Needed for Pass, Traversal, Enum, Node, Nodeset,
    // contains the unique identifier for all data needed in codegen.
    char *hash;
    bool hash_matches;
} NodeCommonInfo;

enum NodeType { NT_node, NT_nodeset };

enum MandatoryPhaseType { MP_single, MP_range };

enum AttrType {
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

enum AttrValueType {
    AV_string,
    AV_int,
    AV_uint,
    AV_float,
    AV_double,
    AV_bool,
    AV_id
};

/**
 * @brief The possible operations to perform on two set expressions. @see
 * SetExpr
 */
enum SetOperator { SET_UNION, SET_INTERSECT, SET_DIFFERENCE };

enum SetExprType { SET_REFERENCE, SET_LITERAL, SET_OPERATION };

enum ActionType {
    ACTION_PASS,
    ACTION_TRAVERSAL,
    ACTION_PHASE,
    ACTION_REFERENCE
};

enum LifetimeType {
    LIFETIME_DISALLOWED,
    LIFETIME_MANDATORY
};

typedef struct ccn_sub_root_pair {
    char *from;
    char *to;
} ccn_sub_root_pair;

typedef struct Phase Phase;
typedef struct Lifetime Lifetime_t;

typedef struct Range_spec {
    array *ids; // Ids of the action this spec starts or end, with namespacing.
    unsigned int id_index;
    bool inclusive; // Range inclusive over the action or not.
    char *consistency_key; // Key used to lookup this spec in the consistency map.
    char *type;
    bool push; // Either push this spec or pop.
    enum LifetimeType life_type;
    uint32_t action_counter_id;
    array *values; // Non owning, lifetime is owner.
    Lifetime_t *owner;
} Range_spec_t;

struct Lifetime {
    Range_spec_t *start;
    Range_spec_t *end;
    enum LifetimeType type;
    char *key;
    bool owner;
    char *original_value;
    array *values;

};

typedef struct Config {
    array *phases;
    array *passes;
    array *traversals;
    array *enums;
    array *nodesets;
    array *nodes;
    array *sub_root_pairs;
    char *header_dir;
    char *source_dir;

    struct Node *root_node;
    struct Phase *start_phase;

    struct NodeCommonInfo *common_info;
} Config;


struct Phase {
    char *id;
    char *info;
    char *prefix;
    char *root;
    char *gate_func;

    bool cycle;
    bool start;
    bool root_owner;

    Phase *original_ref;
    array *actions;
    smap_t *active_specs;
    ccn_set_t *roots;

    struct NodeCommonInfo *common_info;
};

typedef struct Pass {
    char *id;
    char *info;
    char *prefix;
    char *func;
    struct NodeCommonInfo *common_info;
    ccn_set_t *roots;
} Pass;

typedef struct Traversal {
    char *id;
    char *info;
    char *prefix;
    char *func;

    union {
        array *nodes;
        SetExpr *expr;
    };

    struct NodeCommonInfo *common_info;
} Traversal;

typedef struct Action {
    enum ActionType type;
    uint32_t id_counter;
    bool action_owner; // We create shallow actions, that do not own the actual action.
    bool checked;
    void *action;
    char *id;
    smap_t *active_specs;
} Action;

typedef struct Enum {
    char *id;
    char *prefix;
    char *info;

    array *values;

    struct NodeCommonInfo *common_info;
} Enum;

typedef struct Nodeset {
    char *id;
    char *info;

    // The set expr of this nodeset, will be transformed
    // into an array of nodes.
    union {
        array *nodes;
        SetExpr *expr;
    };

    struct NodeCommonInfo *common_info;
} Nodeset;

typedef struct Node {
    char *id;
    char *info;

    // array of (struct Child *)
    array *children;

    // array of (struct Attr *)
    array *attrs;

    array *lifetimes;

    bool root;

    struct NodeCommonInfo *common_info;
} Node;

typedef struct Child {
    int construct;
    int mandatory;
    array *mandatory_phases;
    char *id;
    char *type;

    // One of these becomes a link to the actual child node(set), other NULL
    // after checking the ast.
    struct Node *node;
    struct Nodeset *nodeset;

    array *lifetimes;

    struct NodeCommonInfo *common_info;
} Child;

typedef struct PhaseRange {
    char *start;
    char *end;

    struct NodeCommonInfo *common_info;
} PhaseRange;

typedef struct MandatoryPhase {
    enum MandatoryPhaseType type;
    int negation;

    union {
        struct PhaseRange *range;
        char *single;
    } value;

    struct NodeCommonInfo *common_info;
} MandatoryPhase;

typedef struct Attr {
    int construct;
    enum AttrType type;
    char *type_id;
    char *id;
    struct AttrValue *default_value;
    array *lifetimes;

    struct NodeCommonInfo *common_info;
} Attr;

typedef struct AttrValue {
    enum AttrValueType type;
    union {
        uint64_t uint_value;
        int64_t int_value;
        float float_value;
        double double_value;
        char *string_value;
        bool bool_value;
    } value;

    struct NodeCommonInfo *common_info;
} AttrValue;

/**
 * @struct SetOperation
 * @brief The structure to store a set operation on two expressions.
 *
 * @details A set operation is an operation, like a UNION or INTERSECT
 *  on two set expressions.
 */
typedef struct SetOperation {
    enum SetOperator operator;
    SetExpr *left_child;
    SetExpr *right_child;
    struct NodeCommonInfo *common_info;
} SetOperation;

/**
 * @struct SetExpr
 * @brief The structure to store an internal set expression.
 *
 * @details An internal set expression can be of three possible types:
 *   -# ref_id:
 *        Reference to a defined nodeset.
 *   -# id_set:
 *        A set of node ids, which is defined inline, so by placing between
 *        {} brackets.
 *   -# operation:
 *        A set operation on two node sets. This node set will be the result
 *        of that operation.
 *
 * All these types will be transformed into an array of nodes,
 * which can be found in the nodes array.
 */
struct SetExpr {
    enum SetExprType type;

    union {
        char *ref_id;
        ccn_set_t *id_set;
        SetOperation *operation;
    };

    struct NodeCommonInfo *common_info;
};
