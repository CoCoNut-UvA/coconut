
#include <stddef.h>
#include <stdbool.h>

#include "ccngen/action_handling.h"
#include "ccn/action_types.h"
#include "ccn/dynamic_core.h"
#include "palm/ctinfo.h"

struct phase_driver {
    size_t level;
    size_t action_id;
    struct ccn_phase *current_phase;
};

static struct phase_driver phase_driver = {
    .level = 0,
    .action_id = 0,
    .current_phase = NULL
};

static struct ccn_node *StartPhase(struct ccn_phase *phase, char *phase_name, struct ccn_node *node);

struct ccn_node *CCNdispatchAction(struct ccn_action *action, enum ccn_nodetype root_type, struct ccn_node *node,
                          bool is_root) {
    switch (action->type) {
    case CCN_ACTION_PASS:
        node = PASSstart(node, action->pass.pass_type);
        break;
    case CCN_ACTION_TRAVERSAL:
        node = TRAVstart(node, action->traversal.trav_type);
        break;
    case CCN_ACTION_PHASE:
        node = StartPhase(&(action->phase), action->name, node);
        break;
    }

    phase_driver.action_id++;
    return node;
}



struct ccn_node *StartPhase(struct ccn_phase *phase, char *phase_name, struct ccn_node *node) {
    struct ccn_phase *prev = phase_driver.current_phase;
    phase_driver.current_phase = phase;
    CTIstate("** %s\n", phase_name);
    phase_driver.level++;

    size_t action_counter = 0;
    enum ccn_action_id action_id = phase->action_table[action_counter];
    while (action_id != CCNAC_ID_NULL) {
        struct ccn_action *action = CCNgetActionFromID(action_id);
        node = CCNdispatchAction(action, phase->root_type, node, false);
        action_counter++;
        action_id = phase->action_table[action_counter];
    }

    phase_driver.level--;

    phase_driver.current_phase = prev;
    return node;
}

struct ccn_node *CCNstart(struct ccn_node *node)
{
    return CCNdispatchAction(CCNgetActionFromID(CCN_ROOT_ACTION), CCN_ROOT_TYPE, node, false);
}

