#ifndef _CCN_AST_CORE_H_
#define _CCN_AST_CORE_H_

#include "generated/ast.h"

#define NODE_TYPE(n) ((n)->nodetype)
#define NODE_LINE(n) ((n)->lineno)
#define NODE_COL(n) ((n)->colno)
#define NODE_ERROR(n) ((n)->error)

typedef struct NODE {
    NodeType nodetype;
    int lineno;
    int colno;
    struct NODE *error;
    union NODE_DATA data;
} Node;

Node *node_init();

#endif /* _CCN_AST_CORE_H_ */