#pragma once

#include <stdbool.h>
#include "ccngen/enum.h"
#include "ccngen/action_handling.h"

enum ccn_action_type { CCN_ACTION_PASS, CCN_ACTION_PHASE, CCN_ACTION_TRAVERSAL, CCN_ACTION_NULL};

struct ccn_pass {
    enum ccn_pass_type pass_type;
};

struct ccn_phase {
    bool (*gate_func)(void);
    enum ccn_nodetype root_type;
    enum ccn_action_id *action_table;
    bool is_cycle;
    enum ccn_action_id current_action_id;
};

struct ccn_traversal {
    enum ccn_traversal_type trav_type;
};

struct ccn_action {
    enum ccn_action_type type;
    enum ccn_action_id action_id;
    char *name;
    union {
        struct ccn_traversal traversal;
        struct ccn_pass pass;
        struct ccn_phase phase;
    };
};

struct ccn_action *CCNgetActionFromID(enum ccn_action_id);
