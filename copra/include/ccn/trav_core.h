#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "ccngen/trav.h"

typedef struct TRAV {
    struct TRAV *prev;
    TraversalType trav_type;
    union TRAV_DATA trav_data;
} Trav;

static Trav *current_traversal;

Trav *trav_init_return(Trav *);
Trav *trav_init_error(Trav *);
void trav_free_error(Trav *trav);
void trav_free_return(Trav *trav);
void trav_push(TraversalType trav_type);
void trav_pop();
Trav *trav_current(void);
TraversalType trav_type(void);
Node *trav_start(Node *syntaxtree, TraversalType trav);
Node *trav_return(Node *arg_node);
Node *trav_error(Node *arg_node);
Node *trav_children(Node *arg_node);
Node *trav(Node *arg_node);
Node *trav_opt(Node *arg_node);
Node *node_copy(Node *arg_node);
Node *node_free(Node *arg_node);
#define TRAV_TYPE ((trav_current())->trav_type)

#endif /* _CCN_TRAV_CORE_H_ */