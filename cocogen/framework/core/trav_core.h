#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "core/ast_core.h"

#define TRAV(arg_node, arg_info)                                               \
    if (arg_node != NULL) {                                                    \
        arg_node = traverse_node(arg_node, arg_info);                          \
    }

// Pointer to traversal functions
typedef Node *(*trav_fun_p)(Node *, Info *);

extern Node *traverse_node(Node *arg_node, Info *arg_info);
extern void push_new_traversal(TraversalType traversal);
extern TraversalType pop_cur_traversal(void);
extern const char *traversal_name(void);
extern void trav_set_pre_fun(TraversalType traversal, trav_fun_p prefun);
extern void trav_set_post_fun(TraversalType traversal, trav_fun_p postfun);

#endif /* _CCN_TRAV_CORE_H_ */