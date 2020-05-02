#pragma once

#include "generated/enum.h"
#include "generated/action_handlers.h"
#include "generated/ast.h"

#include "lib/array.h"

#include <stdbool.h>


enum ACTION_TYPE {
    action_pass,
    action_phase,
    action_traversal,
    action_NULL
};

typedef struct ccn_pass {
    void *(*func)(void*, NodeType);
    NodeType root_type;
} ccn_pass_t;

typedef struct ccn_phase {
    bool (*gate_func)(void);
    NodeType root_type;
    enum ACTION_IDS *action_types;
    bool is_cycle;
    enum ACTION_IDS action_id;
} ccn_phase_t;

typedef struct ccn_traversal {
    TraversalType trav_type;
} ccn_traversal_t;

typedef struct ccn_action {
    enum ACTION_TYPE type;
    enum ACTION_IDS action_id;
    char *name;
    union {
        ccn_traversal_t *traversal;
        ccn_pass_t *pass;
        ccn_phase_t *phase;
    };
} ccn_action_t;

ccn_action_t *get_ccn_action_from_id(enum ACTION_IDS);
void *ccn_dispatch_phase(ccn_phase_t *phase, NodeType root_type, void * node, char *name);
void *ccn_dispatch_action(ccn_action_t *action, NodeType root_type, void *node, bool is_root);
