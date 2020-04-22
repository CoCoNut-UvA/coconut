#ifndef _CCN_TRAV_CORE_H_
#define _CCN_TRAV_CORE_H_

#include "generated/enum.h"
#include "generated/trav.h"

void *globaldata[_TRAV_SIZE];

void trav_push(TraversalType trav);
void trav_pop(void);
TraversalType trav_current(void);

#endif /* _CCN_TRAV_CORE_H_ */