#include <stdio.h>

#include "core/trav_core.h"
#include "generated/enum.h"
#include "lib/memory.h"
#include "lib/print.h"

Trav *trav_init(void) {
    Trav *trav = (Trav *)mem_alloc(sizeof(Trav));
    return trav;
}

void trav_free(Trav *trav) { mem_free(trav); }

void trav_push(TravType travtype, Trav *init_data_func(void)) {
    Trav *ts = init_data_func();
    ts->travtype = travtype;
    ts->prev = current_traversal;
    current_traversal = ts;
}

void trav_pop(void free_data_func(Trav *)) {
    if (current_traversal == NULL) {
        print_user_error("traversal-driver",
                         "Cannot pop of empty traversal stack.");
        return;
    }
    Trav *prev = current_traversal->prev;
    free_data_func(current_traversal);
    current_traversal = prev;
}

Trav *trav_current(void) { return current_traversal; }

Node *trav_start(Node *syntaxtree, TravType trav, Trav *init_data_func(void),
                 void free_data_func(Trav *)) {
    trav_push(trav, init_data_func);
    syntaxtree = traverse(syntaxtree);
    trav_pop(free_data_func);
    return syntaxtree;
}