#include "ccn/vtables_core.h"
#include "lib/memory.h"
#include "lib/print.h"

// static Phase *current_phase = NULL;

// void phase_push(PhaseType phase_type) {
//     Phase *phase = (Phase *)mem_alloc(sizeof(Phase));
//     phase->phase_type = phase_type;
//     phase->prev = current_phase;
//     current_phase = phase;
// }

// void phase_pop() {
//     if (current_phase == NULL) {
//         print_user_error("phase-driver", "Cannot pop of empty phase stack.");
//         return;
//     }
//     Phase *prev = current_phase->prev;
//     mem_free(current_phase);
//     current_phase = prev;
// }

Node *pass_start(Node *syntaxtree, PassType pass_type) {
    PassFunc pass_func = pass_vtable[pass_type];
    return pass_func(syntaxtree);
}

Node *pass_error(Node *arg_node) {
    print_user_error("action-driver", "Trying to call unknown pass.");
    return arg_node;
}

// Node *phase_start(Node *syntaxtree, PhaseType phase_type) {
//     phase_push(phase_type);
//     PhaseFunc phase_func = phase_vtable[phase_type];
//     syntaxtree = phase_func(syntaxtree);
//     phase_pop();
//     return syntaxtree;
// }

// Node *phase_error(Node *arg_node) {
//     print_user_error("action-driver", "Trying to call unknown phase.");
//     return arg_node;
// }