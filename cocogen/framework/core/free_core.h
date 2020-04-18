#ifndef _CCN_FREE_CORE_H_
#define _CCN_FREE_CORE_H_

#include "generated/free.h"
#include "trav_core.h"

typedef struct INFO {
    Node *flag;
    Node *assign;
} Info;

#define INFO_FREE_FLAG(n) (n->flag)
#define INFO_FREE_ASSIGN(n) (n->assign)

#define FREETRAV(node, info) (node != NULL) ? traverse_node(node, info) : node
#define FREECOND(node, info)                                                   \
    (INFO_FREE_FLAG(info) != arg_node) ? FREETRAV(node, info) : (node)

static Info *make_info();
static Info *free_info(Info *info);
Node *free_node(Node *syntaxtree);
Node *free_tree(Node *syntaxtree);
char *free_string(char *str);

#endif /* __CCN_FREE_CORE_H_ */