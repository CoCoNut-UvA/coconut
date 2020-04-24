#include <stdio.h>

#include "generated/enum.h"
#include "lib/memory.h"
#include "lib/print.h"

#include "core/trav_core.h"

// Stack of traversals, so that new traversals can be started inside other
// traversals.
typedef struct TRAV_STACK {
    struct TRAV_STACK *prev;
    TraversalType current;
    void *travdata;
} TravStack;

static TravStack *current_traversal;

void trav_push(TraversalType trav, TraversalData *init_data_func(void)) {
    TravStack *new = (TravStack *)mem_alloc(sizeof(TravStack));
    new->travdata = init_data_func();
    new->current = trav;
    new->prev = current_traversal;
    current_traversal = new;
}

void trav_pop(void free_data_func(TraversalData *)) {
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

TraversalType trav_current(void) { return current_traversal->current; }

Node *trav_start(Node *syntaxtree, TraversalType trav,
                 TraversalData *init_data_func(void),
                 void free_data_func(TraversalData *)) {
    trav_push(trav, init_data_func);
    syntaxtree = traverse(syntaxtree);
    trav_pop(free_data_func);
    return syntaxtree;
}
