#ifndef _CCN_AST_CORE_H_
#define _CCN_AST_CORE_H_

#include "generated/ast.h"

typedef Node *(*TravFunc)(Node *);

typedef struct NODE {
    TravFunc trav_func;
    NodeType nodetype;
    int lineno;
    int colno;
    struct NODE *error;
    union NODE_DATA data;

} Node;

Node *node_init();
#define NODE_TYPE(n) ((n)->nodetype)
#define NODE_LINE(n) ((n)->lineno)
#define NODE_COL(n) ((n)->colno)
#define NODE_ERROR(n) ((n)->error)
#define NODE_TRAV(n) ((n)->trav_func(n))

#endif /* _CCN_AST_CORE_H_ */