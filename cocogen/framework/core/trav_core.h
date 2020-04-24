#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "generated/trav.h"

void trav_push(TraversalType trav, TraversalData *travfunc(void));
void trav_pop(void free_data_func(TraversalData *));
TraversalType trav_current(void);
Node *trav_start(Node *syntaxtree, TraversalType trav,
                 TraversalData *init_data_func(void),
                 void free_data_func(TraversalData *));

#endif /* _CCN_TRAV_CORE_H_ */