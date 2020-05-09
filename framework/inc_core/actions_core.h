#ifndef _CCN_ACTIONS_CORE_H_
#define _CCN_ACTIONS_CORE_H_

#include "inc_generated/actions.h"

Node *pass_start(Node *syntaxtree, PassType passtype);
Node *pass_error(Node *arg_node);

#endif /* __CCN_ACTIONS_CORE_H_ */
