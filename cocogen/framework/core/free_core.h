#ifndef _CCN_FREE_CORE_H_
#define _CCN_FREE_CORE_H_

#include "core/trav_core.h"
#include "generated/free.h"

struct FREE_DATA;
typedef struct FREE_DATA FreeData;

FreeData *free_init_data();
void free_data(FreeData *arg_data);
Node *free_node(Node *syntaxtree);
void free_string(char *str);

#endif /* __CCN_FREE_CORE_H_ */