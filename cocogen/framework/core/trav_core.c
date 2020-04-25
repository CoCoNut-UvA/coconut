#include <stdio.h>

#include "core/trav_core.h"
#include "generated/enum.h"
#include "lib/memory.h"
#include "lib/print.h"

// Stack of traversals, so that new traversals can be started inside other
// traversals.
struct TRAV_STACK {
    struct TRAV_STACK *prev;
    TravType travtype;
    TravData *travdata;
};

static TravStack *current_traversal;

void trav_push(TravType trav, TravData *init_data_func(void)) {
    TravStack *new = (TravStack *)mem_alloc(sizeof(TravStack));
    new->travdata = init_data_func();
    new->travtype = trav;
    new->prev = current_traversal;
    current_traversal = new;
}

void trav_pop(void free_data_func(TravData *)) {
    if (current_traversal == NULL) {
        print_user_error("traversal-driver",
                         "Cannot pop of empty traversal stack.");
        return;
    }
    TravStack *prev = current_traversal->prev;
    free_data_func(current_traversal->travdata);
    mem_free(current_traversal);
    current_traversal = prev;
}

TravStack *trav_current(void) { return current_traversal; }
TravData *trav_data(void) { return current_traversal->travdata; }
TravType trav_type(void) { return current_traversal->travtype; }

Node *trav_start(Node *syntaxtree, TravType trav,
                 TravData *init_data_func(void),
                 void free_data_func(TravData *)) {
    trav_push(trav, init_data_func);
    syntaxtree = traverse(syntaxtree);
    trav_pop(free_data_func);
    return syntaxtree;
}
