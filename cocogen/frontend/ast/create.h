#pragma once

#include "lib/array.h"
#include <stdbool.h>
#include <stdint.h>

Config *create_config(array *phases, array *passes, array *traversals,
                      array *attr_enums, array *nodesets, array *nodes);

Pass *create_pass(char *id, char *func, char *prefix);

Traversal *create_traversal(char *id, char *func, char *prefix, SetExpr *expr,
                            array *data);

Phase *create_phase_header(char *id, bool root, bool cycle);

Phase *create_phase(Phase *phase_header, char *root, char *prefix,
                    array *actions, char *gate);

Enum *create_enum(char *id, char *prefix, array *values);

Nodeset *create_nodeset(char *id, SetExpr *expr);

SetOperation *create_set_operation(enum SetOperator operator,
                                   SetExpr *left_child, SetExpr *right_child);

SetExpr *create_set_expr(enum SetExprType type, void *value);

Node *create_node(char *id, Node *nodebody);

Node *create_nodebody(array *children, array *attrs);

Child *create_child(int construct, array *lifetimes, char *id, char *type);

MandatoryPhase *create_mandatory_singlephase(char *phase, int negation);

MandatoryPhase *create_mandatory_phaserange(char *phase_start, char *phase_end,
                                            int negation);

Action *create_action(enum ActionType type, void *action, char *id);

Range_spec_t *create_range_spec(bool inclusive, array *);
Lifetime_t *create_lifetime(Range_spec_t *start, Range_spec_t *end,
                            enum LifetimeType type, array *values);

Attr *create_attr(Attr *attrhead, AttrValue *default_value, int construct,
                  array *lifetimes, char *include);

Attr *create_attrhead_primitive(enum AttrType type, char *id);

Attr *create_attrhead_idtype(char *type, char *id);

AttrValue *create_attrval_string(char *value);

AttrValue *create_attrval_bool(bool value);

AttrValue *create_attrval_int(int64_t value);

AttrValue *create_attrval_uint(uint64_t value);

AttrValue *create_attrval_float(float value);

AttrValue *create_attrval_double(double value);

AttrValue *create_attrval_id(char *id);

Id *create_id(char *);