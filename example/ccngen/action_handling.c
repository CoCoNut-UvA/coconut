#include "ccn/action_types.h"
#include "ccngen/enum.h"
#include "ccngen/action_handling.h"
#include "ccngen/ast.h"
#include "ccn/phase_driver.h"
#include "palm/memory.h"
#include "palm/str.h"
#include <stdbool.h>
enum ccn_action_id main_ids_table[] = {
CCNAC_ID_TRAV, CCNAC_ID_CHANGE, CCNAC_ID_TRAV, CCNAC_ID_P, CCNAC_ID_AGAIN, CCNAC_ID_NULL};

 enum ccn_action_id again_ids_table[] = {
CCNAC_ID_CHANGE, CCNAC_ID_TRAV, CCNAC_ID_NULL};

 enum ccn_action_id cleanup_ids_table[2] = { CCNAC_ID_free, CCNAC_ID_NULL };
static struct ccn_action ccn_action_array[] = {
{CCN_ACTION_PASS, CCNAC_ID_P, "p", .pass = {PASS_P,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_CHANGE, "change", .traversal = {TRAV_CHANGE,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_TRAV, "trav", .traversal = {TRAV_TRAV,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_free, "Free", .traversal = {TRAV_free,},},
{CCN_ACTION_PHASE, CCNAC_ID_MAIN, "main", .phase = {NULL, NT_BINOP, main_ids_table, false, CCNAC_ID_MAIN,},},
{CCN_ACTION_PHASE, CCNAC_ID_AGAIN, "again", .phase = {NULL, NT_BINOP, again_ids_table, false, CCNAC_ID_AGAIN,},},
{CCN_ACTION_PHASE, CCNAC_ID_cleanup, "Cleanup", .phase = {NULL, NT_BINOP, cleanup_ids_table, false, CCNAC_ID_cleanup,},},
};

struct ccn_action *CCNgetActionFromID(enum ccn_action_id action_id) {
return &ccn_action_array[action_id];
}

