#ifndef _CCN_COPY_CORE_H_
#define _CCN_COPY_CORE_H_

#include "core/trav_core.h"
#include "generated/copy.h"

struct COPY_DATA;
typedef struct COPY_DATA CopyData;

CopyData *copy_init_data();
void copy_free_data(CopyData *data);
Node *copy_node(Node *syntaxtree);
char *copy_string(char *str);

#endif /* _CCN_COPY_CORE_H_ */