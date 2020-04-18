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

#define FREETRAV(arg_node, arg_info)                                           \
    (arg_node != NULL) ? traverse(arg_node, arg_info) : arg_node
#define FREECOND(arg_node, arg_info)                                           \
    (INFO_FREE_FLAG(arg_info) != arg_node) ? FREETRAV(arg_node, arg_info)      \
                                           : (arg_node)

static Info *free_make_info();
static Info *free_info(Info *arg_info);
Node *free_node(Node *syntaxtree);
Node *free_tree(Node *syntaxtree);
char *free_string(char *str);

#endif /* __CCN_FREE_CORE_H_ */