#ifndef _CCN_VTABLES_CORE_H_
#define _CCN_VTABLES_CORE_H_

#include "actions_core.h"

typedef Node *(*TravFunc)(Node *);
typedef Trav *(*InitFunc)(Trav *);
typedef void (*FreeFunc)(Trav *);
typedef Node *(*PassFunc)(Node *);
typedef Node *(*PhaseFunc)(Node *);

extern const TravFunc *trav_vtable[_TRAV_SIZE];
extern const InitFunc trav_data_init_vtable[_TRAV_SIZE];
extern const FreeFunc trav_data_free_vtable[_TRAV_SIZE];
extern const PassFunc pass_vtable[_PASS_SIZE];
extern const PhaseFunc phase_vtable[_PHASE_SIZE];

#endif /* __CCN_VTABLES_CORE_H_ */
