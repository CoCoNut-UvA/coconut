#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "generated/trav.h"

typedef struct TRAV {
    struct TRAV *prev;
    TravType travtype;
    union TRAV_DATA travdata;
} Trav;

static Trav *current_traversal;

Trav *trav_init();
void trav_push(TravType travtype, Trav *trav_init(void));
void trav_pop(void trav_free(Trav *));
Trav *trav_current(void);
TravType trav_type(void);
Trav *trav_data(void);
Node *trav_start(Node *syntaxtree, TravType trav, Trav *init_data_func(void),
                 void trav_free(Trav *));
Trav *noop_init(void);
void noop_free(Trav *trav);

#define TRAV_TYPE(trav) ((trav)->travtype)

#endif /* _CCN_TRAV_CORE_H_ */