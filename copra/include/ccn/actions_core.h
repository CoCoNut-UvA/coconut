#ifndef _CCN_ACTIONS_CORE_H_
#define _CCN_ACTIONS_CORE_H_

#include "ccngen/actions.h"

Node *pass_start(Node *syntaxtree, PassType passtype);
Node *pass_error(Node *arg_node);
Node *phase_start(Node *syntaxtree, PhaseType phasetype);
Node *phase_error(Node *arg_node);
Node *start_root_phase();

#endif /* __CCN_ACTIONS_CORE_H_ */
