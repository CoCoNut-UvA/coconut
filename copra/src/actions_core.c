#include "include/vtables_core.h"
#include "lib/print.h"

Node *pass_start(Node *syntaxtree, PassType pass_type) {
    PassFunc pass_func = pass_vtable[pass_type];
    return pass_func(syntaxtree);
}

Node *pass_error(Node *arg_node) {
    print_user_error("action-driver", "Trying to call unknown pass.");
    return arg_node;
}

Node *phase_start(Node *syntaxtree, PhaseType phase_type) {
    PhaseFunc phase_func = phase_vtable[phase_type];
    return phase_func(syntaxtree);
}

Node *phase_error(Node *arg_node) {
    print_user_error("action-driver", "Trying to call unknown phase.");
    return arg_node;
}