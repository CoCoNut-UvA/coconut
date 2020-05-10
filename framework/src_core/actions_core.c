#include <stdio.h>

#include "inc_core/vtables_core.h"
#include "lib/print.h"

Node *pass_start(Node *syntaxtree, PassType pass_type) {
    PassFunc pass_func = pass_vtable[pass_type];
    pass_func(syntaxtree);
}

Node *pass_error(Node *arg_node) {
    print_user_error("action-driver", "Trying to call unknown pass.");
    return arg_node;
}