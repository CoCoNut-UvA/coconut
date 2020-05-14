#ifndef _CCN_AST_CORE_H_
#define _CCN_AST_CORE_H_

#include "include/generated/ast.h"

typedef struct NODE {
    NodeType nodetype;
    union NODE_DATA data;
    struct NODE **children;
    long int numchildren;
} Node;

Node *node_init();

#define NODE_TYPE(n) ((n)->nodetype)
#define NODE_CHILDREN(n) ((n)->children)
#define NODE_NUMCHILDREN(n) ((n)->numchildren)

#endif /* _CCN_AST_CORE_H_ */