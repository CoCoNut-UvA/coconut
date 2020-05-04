#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "generated/trav.h"

typedef struct TRAV {
    struct TRAV *prev;
    TravType travtype;
    union TRAV_DATA travdata;
} Trav;

typedef Node *(*TravFunc)(Node *);
typedef Trav *(*InitFunc)(void);
typedef void (*FreeFunc)(Trav *);

const TravFunc trav_mat[_TRAV_SIZE][_NT_SIZE];
const InitFunc trav_data_init_array[_TRAV_SIZE];
const FreeFunc trav_data_free_array[_TRAV_SIZE];

static Trav *current_traversal;

Trav *trav_init(void);
Trav *trav_init_error(void);
void trav_free_error(Trav *trav);
void trav_free(Trav *trav);
void trav_push(TravType travtype);
void trav_pop();
Trav *trav_current(void);
TravType trav_type(void);
Node *trav_start(Node *syntaxtree, TravType trav);

Node *trav_noop(Node *arg_node);
Node *trav_error(Node *arg_node);
Node *traverse(Node *arg_node);
Node *trav_node(Node *arg_node);
#define TRAV_TYPE ((trav_current())->travtype)

#endif /* _CCN_TRAV_CORE_H_ */