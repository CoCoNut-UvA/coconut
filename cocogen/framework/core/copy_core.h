#ifndef _CCN_COPY_CORE_H_
#define _CCN_COPY_CORE_H_

#include "trav_core.h"

#define COPYTRAV(node, info) (node != NULL) ? traverse(node, info) : node

Node *copy_node(Node *syntaxtree);

#endif /* _CCN_COPY_CORE_H_ */