#ifndef _CCN_COPY_CORE_H_
#define _CCN_COPY_CORE_H_

#include "core/trav_core.h"
#include "generated/copy.h"

Node *copy_node(Node *syntaxtree);
char *copy_string(char *str);

#endif /* _CCN_COPY_CORE_H_ */