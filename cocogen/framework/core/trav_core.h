#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "generated/trav.h"

typedef struct TRAV {
    struct TRAV *prev;
    TravType travtype;
    union TRAV_DATA travdata;
} Trav;

typedef Node *(*TravFunc)(Node *);
const TravFunc trav_mat[_TRAV_SIZE][_NT_SIZE];

static Trav *current_traversal;

Trav *trav_init(void);
void trav_free(Trav *trav);
void trav_push(TravType travtype, Trav *trav_init(void));
void trav_pop(void trav_free(Trav *));
Trav *trav_current(void);
TravType trav_type(void);
Node *trav_start(Node *syntaxtree, TravType trav, Trav *init_data_func(void),
                 void trav_free(Trav *));

Node *trav_noop(Node *arg_node);
Node *trav_error(Node *arg_node);
Node *traverse(Node *arg_node);
#define TRAV_TYPE ((trav_current())->travtype)

#endif /* _CCN_TRAV_CORE_H_ */