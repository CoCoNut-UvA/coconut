#pragma once

#include "ccngen/enum.h"
#include <stddef.h>
enum ccn_action_id {
    CCNAC_ID_P,
    CCNAC_ID_CHANGE,
    CCNAC_ID_TRAV,
    CCNAC_ID_free,
    CCNAC_ID_MAIN,
    CCNAC_ID_AGAIN,
    CCNAC_ID_cleanup,
    CCNAC_ID_NULL,
};

 #ifndef CCN_NUM_OF_ACTIONS
#define CCN_NUM_OF_ACTIONS 5
#endif
#ifndef CCN_ROOT_TYPE
#define CCN_ROOT_TYPE NT_BINOP
#endif
#ifndef CCN_ROOT_ACTION
#define CCN_ROOT_ACTION CCNAC_ID_MAIN
#endif

