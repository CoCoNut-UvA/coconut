#ifndef _CCN_AST_CORE_H_
#define _CCN_AST_CORE_H_

#include "../generated/include/ast.h"

typedef struct NODE {
    NodeType nodetype;
    union NODE_DATA data;
    struct NODE **children;
    long int num_children;
} Node;

Node *node_init();

#define NODE_TYPE(n) ((n)->nodetype)
#define NODE_CHILDREN(n) ((n)->children)
#define NODE_NUMCHILDREN(n) ((n)->num_children)

#endif /* _CCN_AST_CORE_H_ */