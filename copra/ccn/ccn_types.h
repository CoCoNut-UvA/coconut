#pragma once

#include "ccngen/enum.h"
#include <stdbool.h>
#include <stdint.h>

struct ccn_node;
struct ccn_trav;

typedef struct ccn_node *(*ccn_trav_ft)(struct ccn_node *);
typedef struct ccn_node *(*ccn_pass_ft)(struct ccn_node *);
typedef void (*ccn_trav_data_ft)(struct ccn_trav *);

extern const ccn_trav_ft *ccn_trav_vtable[_TRAV_SIZE];
extern const ccn_trav_data_ft trav_data_init_vtable[_TRAV_SIZE];
extern const ccn_trav_data_ft trav_data_free_vtable[_TRAV_SIZE];
extern const ccn_pass_ft ccn_pass_vtable[_PASS_SIZE];
