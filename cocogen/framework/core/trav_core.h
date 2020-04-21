#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "generated/enum.h"
#include "generated/trav.h"
// Stack of traversals, so that new traversals can be started inside other
// traversals.
void trav_push(TraversalType trav);
void trav_pop(void);
TraversalType trav_current(void);

#endif /* _CCN_TRAV_CORE_H_ */