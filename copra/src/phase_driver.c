
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ccn/action_types.h"
#include "ccn/dynamic_core.h"
#include "ccngen/action_handling.h"
#include "palm/ctinfo.h"

struct phase_driver {
    size_t level;
    size_t action_id;
    size_t cycle_iter;
    bool fixed_point;
    struct ccn_phase *current_phase;
};

static struct phase_driver phase_driver = {
    .level = 0,
    .action_id = 0,
    .cycle_iter = 0,
    .current_phase = NULL,
    .fixed_point = false
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
// For now 100, will be configurable later and possible to define a per-cycle config.
#define CCN_MAX_CYCLES 100

struct ccn_node *StartPhase(struct ccn_phase *phase, char *phase_name, struct ccn_node *node) {
    struct ccn_phase *prev = phase_driver.current_phase;
    phase_driver.current_phase = phase;
    if (phase->gate_func && !phase->gate_func()) {
        SkipPhase(phase);
        return node;
    }
    CTIstate("** %s\n", phase_name);
    phase_driver.level++;
    bool cycle = phase->is_cycle;
    uint64_t curr_action_id = phase_driver.action_id;
    do {
        // If we cycle around, reset the action id.
        phase_driver.fixed_point = true;
        phase_driver.action_id = curr_action_id;
        size_t action_counter = 0;
        enum ccn_action_id action_id = phase->action_table[action_counter];
        while (action_id != CCNAC_ID_NULL) {
            struct ccn_action *action = CCNgetActionFromID(action_id);
            node = CCNdispatchAction(action, phase->root_type, node, false);
            action_counter++;
            action_id = phase->action_table[action_counter];
        }
        phase_driver.cycle_iter++;
    } while(cycle && phase_driver.cycle_iter < CCN_MAX_CYCLES && !(phase_driver.fixed_point));

    phase_driver.cycle_iter = 0;
    phase_driver.level--;

    phase_driver.current_phase = prev;
    return node;
}

void CCNcycleNotify()
{
    phase_driver.fixed_point = false;
}

void CCNrun(struct ccn_node *node)
{
    node = CCNdispatchAction(CCNgetActionFromID(CCN_ROOT_ACTION), CCN_ROOT_TYPE, node, false);
    TRAVstart(node, TRAV_free);
}

size_t CCNgetCurrentActionId()
{
    return phase_driver.action_id;
}