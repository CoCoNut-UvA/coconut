#include <stdio.h>

#include "inc_core/vtables_core.h"
#include "lib/print.h"

Node *pass_start(Node *syntaxtree, PassType passtype) {
    pass_vtable[passtype](syntaxtree);
}

Node *pass_error(Node *arg_node) {
    print_user_error("action-driver", "Trying to call unknown pass.");
    return arg_node;
}