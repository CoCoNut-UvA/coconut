#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/create.h"
#include "lib/set_implementations.h"

#include "lib/array.h"
#include "lib/memory.h"
#include "lib/print.h"

extern ParserLocation yy_parser_location;

static NodeCommonInfo *create_commoninfo() {
    NodeCommonInfo *info = (NodeCommonInfo *)mem_alloc(sizeof(NodeCommonInfo));
    info->hash = NULL;
    info->hash_matches = false;
    return info;
}

Config *create_config(array *phases, array *passes, array *traversals,
                      array *enums, array *nodesets, array *nodes) {

    Config *c = mem_alloc(sizeof(Config));
    c->phases = phases;
    c->passes = passes;
    c->traversals = traversals;
    c->enums = enums;
    c->nodesets = nodesets;
    c->nodes = nodes;
    c->header_dir = NULL;
    c->source_dir = NULL;
    c->sub_root_pairs = create_array();

    c->common_info = create_commoninfo();
    return c;
}

Phase *create_phase_header(char *id, bool start, bool cycle) {
    Phase *p = mem_alloc(sizeof(Phase));
    p->id = id;
    p->info = NULL;
    p->root = NULL;
    p->roots = ccn_set_string_create(2);
    p->start = start;
    p->cycle = cycle;
    p->root_owner = true;
    p->original_ref = NULL;
    p->gate_func = NULL;

    p->common_info = create_commoninfo();
    return p;
}

Phase *create_phase(Phase *phase_header, char *root, char *prefix,
                    array *actions, char *gate_func) {

    Phase *p = phase_header;
    p->root = root;
    p->actions = actions;
    p->prefix = prefix;
    p->gate_func = gate_func;
    p->active_specs = smap_init(5);
    return p;
}

Pass *create_pass(char *id, char *func, char *prefix) {
    Pass *p = mem_alloc(sizeof(Pass));

    p->id = id;
    p->func = func;
    p->info = NULL;
    p->prefix = prefix;
    p->roots = ccn_set_string_create(5);

    p->common_info = create_commoninfo();
    return p;
}

Traversal *create_traversal(char *id, char *func, char *prefix, SetExpr *expr,
                            array *data) {

    Traversal *t = mem_alloc(sizeof(Traversal));
    t->id = id;
    t->func = func;
    t->info = NULL;
    t->prefix = prefix;
    t->expr = expr;
    t->data = data;

    t->common_info = create_commoninfo();
    return t;
}

Enum *create_enum(char *id, char *prefix, array *values) {

    Enum *e = mem_alloc(sizeof(Enum));

    e->id = id;
    e->values = values;
    e->prefix = prefix;
    e->info = NULL;

    e->common_info = create_commoninfo();
    return e;
}

Nodeset *create_nodeset(char *id, SetExpr *expr) {
    Nodeset *n = mem_alloc(sizeof(Nodeset));
    n->id = id;
    n->expr = expr;
    n->info = NULL;

    n->common_info = create_commoninfo();
    return n;
}

SetOperation *create_set_operation(enum SetOperator operator,
                                   SetExpr *left_child, SetExpr *right_child) {
    SetOperation *operation = mem_alloc(sizeof(SetOperation));
    operation->operator= operator;
    operation->left_child = left_child;
    operation->right_child = right_child;

    operation->common_info = create_commoninfo();
    return operation;
}

/**
 * @brief Transform an id list to a set of characters.
 *        The array will be destroyed.
 */
ccn_set_t *idlist_to_set(array *ids) {
    ccn_set_t *set = ccn_set_string_create(20);

    for (int i = 0; i < array_size(ids); i++) {
        void *item = array_get(ids, i);
        char *id = (char *)item;
        if (!ccn_set_insert(set, item)) {
            char *id_def = (char *)ccn_set_get(set, item);
            print_warning(
                id, "Set element is already defined, so will be ignored.");
            print_note(id_def, "Set element already defined here.");
        }
    }
    array_clear(ids);
    array_cleanup(ids, NULL);

    return set;
}

SetExpr *create_set_expr(enum SetExprType type, void *value) {
    SetExpr *expr = mem_alloc(sizeof(SetExpr));
    expr->type = type;

    switch (type) {
    case SET_REFERENCE:
        expr->ref_id = (char *)value;
        break;
    case SET_LITERAL:
        expr->id_set = idlist_to_set((array *)value);
        break;
    case SET_OPERATION:
        expr->operation = (SetOperation *)value;
        break;
    default:
        // TODO better exit.
        printf("No such type for a set expression.");
        exit(1);
    }
    expr->common_info = create_commoninfo();
    return expr;
}

Node *create_node(char *id, Node *nodebody) {

    nodebody->id = id;
    nodebody->root = false;
    return nodebody;
}

Node *create_nodebody(array *children, array *attrs) {
    Node *n = mem_alloc(sizeof(Node));
    n->children = children;
    n->attrs = attrs;
    n->info = NULL;
    n->lifetimes = NULL;

    n->common_info = create_commoninfo();
    return n;
}

Range_spec_t *create_range_spec(bool inclusive, array *ids) {
    Range_spec_t *spec = mem_alloc(sizeof(Range_spec_t));
    spec->inclusive = inclusive;
    spec->ids = ids;
    spec->id_index = 0;
    spec->action_counter_id = 0;
    spec->values = NULL;
    return spec;
}

Lifetime_t *create_lifetime(Range_spec_t *start, Range_spec_t *end,
                            enum LifetimeType type, array *values) {
    Lifetime_t *lifetime = mem_alloc(sizeof(Lifetime_t));
    lifetime->start = start;
    if (lifetime->start != NULL) {
        lifetime->start->push = true;
        lifetime->start->owner = lifetime;
    }
    lifetime->end = end;
    if (lifetime->end != NULL) {
        lifetime->end->push = false;
        lifetime->end->owner = lifetime;
    }
    lifetime->type = type;
    lifetime->key = NULL;
    lifetime->values = values;
    lifetime->owner = true;
    return lifetime;
}

Child *create_child(int construct, array *lifetimes, char *id, char *type) {

    Child *c = mem_alloc(sizeof(Child));
    c->construct = construct;
    c->id = id;
    c->type = type;

    c->lifetimes = lifetimes;
    c->node = NULL;
    c->nodeset = NULL;

    c->common_info = create_commoninfo();
    return c;
}

MandatoryPhase *create_mandatory_singlephase(char *phase, int negation) {

    MandatoryPhase *p = mem_alloc(sizeof(MandatoryPhase));
    p->value.single = phase;
    p->type = MP_single;
    p->negation = negation;

    p->common_info = create_commoninfo();
    return p;
}

MandatoryPhase *create_mandatory_phaserange(char *phase_start, char *phase_end,
                                            int negation) {

    MandatoryPhase *p = mem_alloc(sizeof(MandatoryPhase));
    PhaseRange *range = mem_alloc(sizeof(PhaseRange));
    range->start = phase_start;
    range->end = phase_end;
    p->value.range = range;
    p->type = MP_range;
    p->negation = negation;

    p->common_info = create_commoninfo();
    return p;
}

Action *create_action(enum ActionType type, void *action, char *id) {
    Action *_action = mem_alloc(sizeof(Action));
    _action->type = type;
    _action->action = action;
    _action->checked = false;
    _action->id = strdup(id);
    _action->active_specs = smap_init(2);
    _action->action_owner = true;
    _action->id_counter = 0;
    return _action;
}

TravData *create_travdata_primitive(enum AttrType type, char *id,
                                    AttrValue *value) {

    TravData *td = mem_alloc(sizeof(TravData));
    td->type = type;
    td->type_id = NULL;
    td->id = id;
    td->value.primitive_value = value;

    td->common_info = create_commoninfo();
    return td;
}

TravData *create_travdata_struct(char *type, char *id, char *include) {

    TravData *td = mem_alloc(sizeof(TravData));
    td->type = AT_link_or_enum;
    td->type_id = type;
    td->id = id;
    td->value.include = include;

    td->common_info = create_commoninfo();
    return td;
}

Attr *create_attr(Attr *a, AttrValue *default_value, int construct,
                  array *lifetimes) {
    a->default_value = default_value;
    a->construct = construct;
    a->lifetimes = lifetimes;
    return a;
}

Attr *create_attrhead_primitive(enum AttrType type, char *id) {

    Attr *a = mem_alloc(sizeof(Attr));
    a->type = type;
    a->type_id = NULL;
    a->id = id;

    a->common_info = create_commoninfo();
    return a;
}

Attr *create_attrhead_idtype(char *type, char *id) {

    Attr *a = mem_alloc(sizeof(Attr));
    a->type = AT_link_or_enum;
    a->type_id = type;
    a->id = id;

    a->common_info = create_commoninfo();
    return a;
}

AttrValue *create_attrval_string(char *value) {
    AttrValue *v = mem_alloc(sizeof(AttrValue));
    v->type = AV_string;

    v->value.string_value = value;

    v->common_info = create_commoninfo();
    return v;
}

AttrValue *create_attrval_int(int64_t value) {
    AttrValue *v = mem_alloc(sizeof(AttrValue));
    v->type = AV_int;
    v->value.int_value = value;

    v->common_info = create_commoninfo();
    return v;
}

AttrValue *create_attrval_uint(uint64_t value) {
    AttrValue *v = mem_alloc(sizeof(AttrValue));
    v->type = AV_uint;
    v->value.uint_value = value;

    v->common_info = create_commoninfo();
    return v;
}

AttrValue *create_attrval_float(float value) {
    AttrValue *v = mem_alloc(sizeof(AttrValue));
    v->type = AV_float;
    v->value.float_value = value;

    v->common_info = create_commoninfo();
    return v;
}

AttrValue *create_attrval_double(double value) {
    AttrValue *v = mem_alloc(sizeof(AttrValue));
    v->type = AV_double;
    v->value.double_value = value;

    v->common_info = create_commoninfo();
    return v;
}

AttrValue *create_attrval_bool(bool value) {
    AttrValue *v = mem_alloc(sizeof(AttrValue));
    v->type = AV_bool;
    v->value.bool_value = value;

    v->common_info = create_commoninfo();
    return v;
}

AttrValue *create_attrval_id(char *id) {
    AttrValue *v = mem_alloc(sizeof(AttrValue));
    v->type = AV_id;
    v->value.string_value = id;

    v->common_info = create_commoninfo();
    return v;
}
