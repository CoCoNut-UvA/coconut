#ifndef _CCN_FREE_CORE_H_
#define _CCN_FREE_CORE_H_

typedef struct INFO {
    Node *flag;
    Node *assign;
} Info;

#define INFO_FREE_FLAG(n) (n->flag)
#define INFO_FREE_ASSIGN(n) (n->assign)

#define FREETRAV(node, info) (node != NULL) ? TRAVdo(node, info) : node
#define FREECOND(node, info)                                                   \
    (INFO_FREE_FLAG(info) != arg_node) ? FREETRAV(node, info) : (node)

#endif /* __CCN_FREE_CORE_H_ */