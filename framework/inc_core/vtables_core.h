#ifndef _CCN_VTABLES_CORE_H_
#define _CCN_VTABLES_CORE_H_

#include "inc_core/ast_core.h"

typedef Node *(*TravFunc)(Node *);
typedef Trav *(*InitFunc)(void);
typedef void (*FreeFunc)(Trav *);
typedef Node *(*PassFunc)(Node *);

extern const TravFunc *trav_mat[_TRAV_SIZE];
extern const InitFunc travdata_init_vtable[_TRAV_SIZE];
extern const FreeFunc travdata_free_vtable[_TRAV_SIZE];
extern const PassFunc pass_vtable[_PASS_SIZE];

TravFunc get_travfunc(TravType, NodeType);
InitFunc get_initfunc(TravType);
FreeFunc get_freefunc(TravType);
PassFunc get_passfunc(PassType);

#endif /* __CCN_VTABLES_CORE_H_ */
