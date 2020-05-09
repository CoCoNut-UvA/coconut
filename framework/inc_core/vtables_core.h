#include "inc_core/ast_core.h"

typedef Node *(*TravFunc)(Node *);
typedef Trav *(*InitFunc)(void);
typedef void (*FreeFunc)(Trav *);

const TravFunc *trav_mat[_TRAV_SIZE];
const InitFunc trav_data_init_array[_TRAV_SIZE];
const FreeFunc trav_data_free_array[_TRAV_SIZE];
