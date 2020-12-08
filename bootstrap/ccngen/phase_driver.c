
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
    phase_driver.action_id++;
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

    // TODO: wrap in an ifdef to check for CHECK_ENABLED.
    node = TRAVstart(node, TRAV_check);
    CTIabortOnError();

    return node;
}

static void SkipPhase(struct ccn_phase *phase) {
    size_t action_counter = 0;
    enum ccn_action_id action_id = phase->action_table[action_counter];
    while (action_id != CCNAC_ID_NULL) {
        phase_driver.action_id++;
        action_counter++;
        action_id = phase->action_table[action_counter];
    }
}

struct ccn_node *StartPhase(struct ccn_phase *phase, char *phase_name, struct ccn_node *node) {
    struct ccn_phase *prev = phase_driver.current_phase;
    phase_driver.current_phase = phase;
    if (phase->gate_func && !phase->gate_func()) {
        printf("NO!!!!!!!!!!!!!!\n");
        SkipPhase(phase);
        return node;
    }
    printf("NO! BUT YES\n");
    CTIstate("** %s\n", phase_name);
    phase_driver.level++;
    bool cycle = phase->is_cycle;
    do {
        size_t action_counter = 0;
        enum ccn_action_id action_id = phase->action_table[action_counter];
        while (action_id != CCNAC_ID_NULL) {
            struct ccn_action *action = CCNgetActionFromID(action_id);
            node = CCNdispatchAction(action, phase->root_type, node, false);
            action_counter++;
            action_id = phase->action_table[action_counter];
        }
    } while(cycle);

    phase_driver.level--;

    phase_driver.current_phase = prev;
    return node;
}

struct ccn_node *CCNstart(struct ccn_node *node)
{
    return CCNdispatchAction(CCNgetActionFromID(CCN_ROOT_ACTION), CCN_ROOT_TYPE, node, false);
}

size_t CCNgetCurrentActionId()
{
    return phase_driver.action_id;
}