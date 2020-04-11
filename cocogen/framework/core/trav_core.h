#include "core/ast_core.h"

#define TRAV(arg_node, arg_info)                                               \
    if (arg_node != NULL) {                                                    \
        arg_node = traverse_node(arg_node, arg_info);                          \
    }

typedef struct INFO Info;
typedef Node *(*trav_fun)(Node *, Info *);
Node *traverse_node(Node *arg_node, Info *arg_info);
Node *traverse_children(Node *arg_node, Info *arg_info);
void push_new_traversal(TraversalType prefix);
TraversalType pop_cur_traversal(void);
const char *traversal_name(void);