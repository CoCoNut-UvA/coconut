#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "include/generated/trav_data.h"

typedef struct TRAV {
    struct TRAV *prev;
    TravType trav_type;
    union TRAV_DATA trav_data;
} Trav;

static Trav *current_traversal;

Trav *trav_init(void);
Trav *trav_init_error(void);
void trav_free_error(Trav *trav);
void trav_free(Trav *trav);
void trav_push(TravType trav_type);
void trav_pop();
Trav *trav_current(void);
TravType trav_type(void);
Node *trav_start(Node *syntaxtree, TravType trav);
Node *trav_return(Node *arg_node);
Node *trav_error(Node *arg_node);
Node *trav_children(Node *arg_node);
Node *traverse(Node *arg_node);
#define TRAV_TYPE ((trav_current())->trav_type)

#endif /* _CCN_TRAV_CORE_H_ */