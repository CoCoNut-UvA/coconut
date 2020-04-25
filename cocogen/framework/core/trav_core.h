#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "generated/trav.h"

typedef struct TRAV_STACK TravStack;

void trav_push(TravType trav, TravData *travfunc(void));
void trav_pop(void free_data_func(TravData *));
TravStack *trav_current(void);
TravType trav_type(void);
TravData *trav_data(void);
Node *trav_start(Node *syntaxtree, TravType trav,
                 TravData *init_data_func(void),
                 void free_data_func(TravData *));

#endif /* _CCN_TRAV_CORE_H_ */