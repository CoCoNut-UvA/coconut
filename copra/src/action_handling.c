#include <string.h>
#include <time.h>

#include "ccngen/action_handlers.h"
#include "ccngen/ccn_enables.h"
// #include "ccngen/trav-ast.h"
#include "ccn/action_handling.h"
#include "ccn/actions_core.h"
#include "ccn/internal_phase_functions.h"
//#include "ccngen/_sub_root_handlers.h"

// TODO: Important! Make sure changes of nodes and traversals is handled
// correctly.

void *ccn_run_phase_actions(ccn_phase_t *phase, char *name, NodeType root,
                            void *node);

array *generate_mark_array_from_values_array(array *values) {
    array *marks = create_array();
    for (int i = 0; i < array_size(values); ++i) {
        void *val = array_get(values, i);
        array_append(marks, _ccn_new_mark(val));
    }

    return marks;
}

array *generate_sub_roots_using_offset(void *main, size_t offset) {
    array *values = create_array();

    do {
        char *base = (char *)main;
        void **ref = (void **)(base + offset);
        void *next = *ref;
        *ref = NULL;
        array_append(values, main);
        main = next;
    } while (main != NULL);

    return values;
}

void *collapse_sub_root_array(array *values, size_t offset) {
    void *curr = array_get(values, 0);
    void *first = curr;
    if (array_size(values) == 1)
        return curr;

    size_t index = 1;
    const size_t size = array_size(values);
    while (index < size) {
        void *next = array_get(values, index);
        index++;
        char *base = (char *)curr;
        void **ref = (void **)(base + offset);
        *ref = next;
        curr = next;
    }
    array_cleanup(values, NULL);
    return first;
}

void ccn_cycle_mark_array(ccn_phase_t *phase, array *marks, NodeType root,
                          char *name) {
    bool all_notified = false;
    phase_driver_t *pd = _get_phase_driver();
    const int action_id = pd->action_id;
    while (!all_notified) {
        pd->action_id = action_id;
        _reset_cycle();
        all_notified = true;
        for (int i = 0; i < array_size(marks); ++i) {
            cycle_mark_t *mark = array_get(marks, i);
            if (!mark->notified) {
                continue;
            }
            all_notified = false;
            mark->notified = false;
            _ccn_set_curr_mark(mark);
            ccn_run_phase_actions(phase, name, root, mark->node);
        }
#ifdef CCN_ENABLE_POINTS
        _ccn_check_points(phase->action_id, name);
#endif
    }
}

void *ccn_run_phase_actions(ccn_phase_t *phase, char *name, NodeType root,
                            void *node) {
    size_t index = 0;
    phase_driver_t *pd = _get_phase_driver();
    while (phase->action_types[index] != ACTION_ID_NULL) {
        enum ACTION_IDS action_id = phase->action_types[index];
        ccn_action_t *action = get_ccn_action_from_id(action_id);
        pd->current_action = action->name;
        node = ccn_dispatch_action(action, root, node, false);
#ifdef CCN_ENABLE_POINTS
        _ccn_check_points(action_id, action->name);
#endif
        _exit_on_action_error(action->name);
        index++;
    }
    return node;
}

static void *handle_sub_root(ccn_phase_t *phase, void *node, NodeType root_type,
                             char *name) {

#if 0
    NodeType new_root = phase->root_type;
    void *temp = NULL;

    TraversalType trav =
        get_sub_root_find_traversals(root_type, phase->root_type);
    if (trav == TRAV_NULL) { // TODO: Signal error here, maybe only in dbug,
                             // with the new dbug options in ccn lib.
        return node;
    }

    //dispatch_traversals(root_type, node, trav);
    temp = node;
    phase_driver_t *pd = _get_phase_driver();

    return node;
    if (pd->curr_sub_root != NULL) {
        if (pd->curr_sub_root->nodetype != new_root) {
            return node;
        }
        node = pd->curr_sub_root->value;
    } else {
        return node;
    }

    size_t offset = get_offset_next(new_root);
    array *vals = generate_sub_roots_using_offset(node, offset);

    if (phase->is_cycle) {
        array *marks = generate_mark_array_from_values_array(vals);
        ccn_cycle_mark_array(phase, marks, new_root, name);
        array_cleanup(marks, mem_free);
        collapse_sub_root_array(vals, offset);
    } else {
        for (int i = 0; i < array_size(vals); ++i) {
            void *val = array_get(vals, i);
            ccn_run_phase_actions(phase, name, new_root, val);
        }

        collapse_sub_root_array(vals, offset);
    }

    if (temp) {
        return temp;
    } else {
        return node;
    }
#endif
}

void *ccn_dispatch_phase(ccn_phase_t *phase, NodeType root_type, void *node,
                         char *name) {

    _ccn_start_phase(name, root_type);
    if (phase->root_type != CCN_ROOT_TYPE) {
        node = handle_sub_root(phase, node, root_type, name);
    } else {
        if (phase->is_cycle) {
            array *marks = array_init(1);
            array_append(marks, _ccn_new_mark(node));
            ccn_cycle_mark_array(phase, marks, root_type, name);
            array_cleanup(marks, mem_free);
        } else {
            node = ccn_run_phase_actions(phase, name, root_type, node);
        }
    }
    _exit_on_phase_error();
    _ccn_end_phase(name);

    return node;
}

void *ccn_dispatch_action(ccn_action_t *action, NodeType root_type, void *node,
                          bool is_root) {
    double start, end;
    phase_driver_t *pd = _get_phase_driver();
#ifdef CCN_ENABLE_CHECKS
    dispatch_traversals(root_type, node, TRAV__CCN_CHK);
#endif
    switch (action->type) {
    case action_pass:
        start = clock();
        node = pass_start(node, action->pass.pass_type);
        end = clock();
        _ccn_new_pass_time_frame(action->name, (end - start) / CLOCKS_PER_SEC);
        break;
    case action_traversal:
        start = clock();
        node = trav_start(node, action->traversal.trav_type);
        end = clock();
        _ccn_new_pass_time_frame(action->name, (end - start) / CLOCKS_PER_SEC);
        break;
    case action_phase:
        start = clock();
        node = ccn_dispatch_phase(&action->phase, root_type, node, action->name);
        end = clock();
        if (!is_root) {
            _ccn_new_phase_time_frame(action->name,
                                      (end - start) / CLOCKS_PER_SEC);
        }
        break;
    }

    pd->action_id++;
    return node;
}
