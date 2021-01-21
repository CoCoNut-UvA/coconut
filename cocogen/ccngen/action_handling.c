#include <stddef.h>
#include "ccn/action_types.h"
#include "ccngen/action_handling.h"
#include "ccngen/enum.h"
#include "ccngen/ast.h"
#include "ccngen/gate.h"
#include "ccn/phase_driver.h"
enum ccn_action_id cleanup_ids_table[2] = { CCNAC_ID_free, CCNAC_ID_NULL };
enum ccn_action_id dynamic_gen_user_files_ids_table[] = {
CCNAC_ID_DYNAMIC_USER_GEN_PASSES, CCNAC_ID_DYNAMIC_USER_GEN_TRAVERSALS, CCNAC_ID_NULL, };

enum ccn_action_id dynamic_gen_ast_ids_table[] = {
CCNAC_ID_DYNAMIC_START_AST_HEADER, CCNAC_ID_DYNAMIC_GENNODESTRUCTS, CCNAC_ID_DYNAMIC_GENNODEMACROS, CCNAC_ID_DYNAMIC_GENNODEINITFUNCTIONS, CCNAC_ID_DYNAMIC_GENNODEUNION, CCNAC_ID_DYNAMIC_GENBASENODE, CCNAC_ID_DYNAMIC_SWITCH_TO_AST_SOURCE, CCNAC_ID_DYNAMIC_GENBASENODEINIT, CCNAC_ID_DYNAMIC_GENNODECONSTRUCTOR, CCNAC_ID_NULL, };

enum ccn_action_id dynamic_gen_trav_data_ids_table[] = {
CCNAC_ID_DYNAMIC_START_TRAV_DATA, CCNAC_ID_DYNAMIC_GENTRAVDATASTRUCTS, CCNAC_ID_DYNAMIC_GENTRAVDATAUNION, CCNAC_ID_DYNAMIC_GENTRAVDATAGETTERS, CCNAC_ID_DYNAMIC_GENTRAVDATACTORSHEADER, CCNAC_ID_DYNAMIC_SWITCH_TRAV_DATA, CCNAC_ID_DYNAMIC_GENTRAVDATACTORS, CCNAC_ID_NULL, };

enum ccn_action_id static_gen_ast_ids_table[] = {
CCNAC_ID_STATIC_GEN_NODE_STRUCTS, CCNAC_ID_NULL, };

enum ccn_action_id static_backend_ids_table[] = {
CCNAC_ID_STATIC_GEN_AST, CCNAC_ID_NULL, };

enum ccn_action_id dynamic_backend_ids_table[] = {
CCNAC_ID_DYNAMIC_GEN_AST, CCNAC_ID_DYNAMIC_START_ENUM_HEADER, CCNAC_ID_DYNAMIC_GENENUM, CCNAC_ID_DYNAMIC_GENENUMVALUES, CCNAC_ID_DYNAMIC_GEN_TRAV_DATA, CCNAC_ID_DYNAMIC_START_TABLE_GEN, CCNAC_ID_DYNAMIC_GENTRAVVTABLE, CCNAC_ID_DYNAMIC_GENPASSTABLE, CCNAC_ID_DYNAMIC_GENSYSTEMTRAVERSALS, CCNAC_ID_DYNAMIC_GENTRAVTABLE, CCNAC_ID_DYNAMIC_GENTRAVDATATABLES, CCNAC_ID_DYNAMIC_GENACTIONSHEADER, CCNAC_ID_DYNAMIC_GENCOPYTRAVERSAL, CCNAC_ID_DYNAMIC_GENFREETRAVERSAL, CCNAC_ID_DYNAMIC_GENCHECKTRAVERSAL, CCNAC_ID_DYNAMIC_GEN_USER_FILES, CCNAC_ID_NULL, };

enum ccn_action_id phase_driver_ids_table[] = {
CCNAC_ID_GEN_ACTION_ARRAY_HEADER, CCNAC_ID_GEN_IDS_TABLES, CCNAC_ID_GEN_ACTION_ARRAY, CCNAC_ID_GEN_GATE_HEADER, CCNAC_ID_NULL, };

enum ccn_action_id unpack_lifetimes_ids_table[] = {
CCNAC_ID_ASSIGN_ID_TO_ACTION, CCNAC_ID_MAP_ID_TO_LIFETIME, CCNAC_ID_NULL, };

enum ccn_action_id semantic_ids_table[] = {
CCNAC_ID_BUILDSYMBOLTABLE, CCNAC_ID_CHECKEXISTANCE, CCNAC_ID_UNWRAPSETEXPR, CCNAC_ID_REACHABILITY, CCNAC_ID_NULL, };

enum ccn_action_id opts_ids_table[] = {
CCNAC_ID_DOOPTS, CCNAC_ID_NULL, };

enum ccn_action_id startphase_ids_table[] = {
CCNAC_ID_SCANANDPARSE, CCNAC_ID_SEMANTIC, CCNAC_ID_UNPACK_LIFETIMES, CCNAC_ID_DYNAMIC_BACKEND, CCNAC_ID_STATIC_BACKEND, CCNAC_ID_PHASE_DRIVER, CCNAC_ID_GEN_DOT, CCNAC_ID_GENDEFINES, CCNAC_ID_PRINT, CCNAC_ID_NULL, };

static struct ccn_action ccn_action_array[] = {
{CCN_ACTION_PHASE, CCNAC_ID_DYNAMIC_GEN_USER_FILES, "dynamic_gen_user_files", .phase = {NULL, NT_AST, dynamic_gen_user_files_ids_table, false, CCNAC_ID_DYNAMIC_GEN_USER_FILES,},},
{CCN_ACTION_PHASE, CCNAC_ID_DYNAMIC_GEN_AST, "dynamic_gen_ast", .phase = {NULL, NT_AST, dynamic_gen_ast_ids_table, false, CCNAC_ID_DYNAMIC_GEN_AST,},},
{CCN_ACTION_PHASE, CCNAC_ID_DYNAMIC_GEN_TRAV_DATA, "dynamic_gen_trav_data", .phase = {NULL, NT_AST, dynamic_gen_trav_data_ids_table, false, CCNAC_ID_DYNAMIC_GEN_TRAV_DATA,},},
{CCN_ACTION_PHASE, CCNAC_ID_STATIC_GEN_AST, "static_gen_ast", .phase = {NULL, NT_AST, static_gen_ast_ids_table, false, CCNAC_ID_STATIC_GEN_AST,},},
{CCN_ACTION_PHASE, CCNAC_ID_STATIC_BACKEND, "static_backend", .phase = {backendIsStatic, NT_AST, static_backend_ids_table, false, CCNAC_ID_STATIC_BACKEND,},},
{CCN_ACTION_PHASE, CCNAC_ID_DYNAMIC_BACKEND, "dynamic_backend", .phase = {backendIsDynamic, NT_AST, dynamic_backend_ids_table, false, CCNAC_ID_DYNAMIC_BACKEND,},},
{CCN_ACTION_PHASE, CCNAC_ID_PHASE_DRIVER, "phase_driver", .phase = {NULL, NT_AST, phase_driver_ids_table, false, CCNAC_ID_PHASE_DRIVER,},},
{CCN_ACTION_PHASE, CCNAC_ID_UNPACK_LIFETIMES, "unpack_lifetimes", .phase = {NULL, NT_AST, unpack_lifetimes_ids_table, false, CCNAC_ID_UNPACK_LIFETIMES,},},
{CCN_ACTION_PHASE, CCNAC_ID_SEMANTIC, "semantic", .phase = {NULL, NT_AST, semantic_ids_table, false, CCNAC_ID_SEMANTIC,},},
{CCN_ACTION_PHASE, CCNAC_ID_OPTS, "opts", .phase = {NULL, NT_AST, opts_ids_table, true, CCNAC_ID_OPTS,},},
{CCN_ACTION_PHASE, CCNAC_ID_STARTPHASE, "startPhase", .phase = {NULL, NT_AST, startphase_ids_table, false, CCNAC_ID_STARTPHASE,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_PRINT, "print", .traversal = {TRAV_PRT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_SETLITERALCONTAINS, "setliteralContains", .traversal = {TRAV_SLC,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_SETLITERALDIFFERENCE, "setliteralDifference", .traversal = {TRAV_SLD,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_SETLITERALINTERSECT, "setliteralIntersect", .traversal = {TRAV_SLIS,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_SETLITERALUNION, "setliteralUnion", .traversal = {TRAV_SLU,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_SETLITERALINSERT, "setliteralInsert", .traversal = {TRAV_SLI,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_REACHABILITY, "reachability", .traversal = {TRAV_RCB,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_UNWRAPSETEXPR, "unwrapSetExpr", .traversal = {TRAV_USE,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_BUILDSYMBOLTABLE, "buildSymbolTable", .traversal = {TRAV_BST,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_CHECKEXISTANCE, "checkExistance", .traversal = {TRAV_CEX,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENNODEMACROS, "dynamic_genNodeMacros", .traversal = {TRAV_DGNM,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENNODESTRUCTS, "dynamic_genNodeStructs", .traversal = {TRAV_DGNS,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENNODEINITFUNCTIONS, "dynamic_genNodeInitFunctions", .traversal = {TRAV_DGIF,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENNODEUNION, "dynamic_genNodeUnion", .traversal = {TRAV_DGBU,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENNODECONSTRUCTOR, "dynamic_genNodeConstructor", .traversal = {TRAV_DGNC,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENENUM, "dynamic_genEnum", .traversal = {TRAV_DGE,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENENUMVALUES, "dynamic_genEnumValues", .traversal = {TRAV_DGEV,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVVTABLE, "dynamic_genTravVtable", .traversal = {TRAV_DGTV,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENNODESFORTRAVTABLE, "dynamic_genNodesForTravTable", .traversal = {TRAV_DGNFTT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENPASSTABLE, "dynamic_genPassTable", .traversal = {TRAV_DGPT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVTABLE, "dynamic_genTravTable", .traversal = {TRAV_DGTT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENSYSTEMTRAVERSALS, "dynamic_genSystemTraversals", .traversal = {TRAV_DGST,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENACTIONSHEADER, "dynamic_genActionsHeader", .traversal = {TRAV_DGAH,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVACTIONS, "dynamic_genTravActions", .traversal = {TRAV_DGTA,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENCOPYTRAVERSAL, "dynamic_genCopyTraversal", .traversal = {TRAV_DGCT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENCONSTRUCTORCALL, "dynamic_genConstructorCall", .traversal = {TRAV_DGCC,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENFREETRAVERSAL, "dynamic_genFreeTraversal", .traversal = {TRAV_DGFT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENCHECKTRAVERSAL, "dynamic_genCheckTraversal", .traversal = {TRAV_DGCHT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVDATASTRUCTS, "dynamic_genTravDataStructs", .traversal = {TRAV_DGTDS,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVDATACTORS, "dynamic_genTravDataCtors", .traversal = {TRAV_DGTDC,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVDATACTORSHEADER, "dynamic_genTravDataCtorsHeader", .traversal = {TRAV_DGTDCH,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVDATAGETTERS, "dynamic_genTravDataGetters", .traversal = {TRAV_DGTDG,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVDATAUNION, "dynamic_genTravDataUnion", .traversal = {TRAV_DGTDU,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_GEN_ACTION_ARRAY, "gen_action_array", .traversal = {TRAV_GAA,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_GEN_ACTION_ARRAY_HEADER, "gen_action_array_header", .traversal = {TRAV_GAAH,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_GEN_IDS_TABLES, "gen_ids_tables", .traversal = {TRAV_GIT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_GEN_GATE_HEADER, "gen_gate_header", .traversal = {TRAV_GGH,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_GENTRAVDATATABLES, "dynamic_genTravDataTables", .traversal = {TRAV_DGTDT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_GEN_DOT, "gen_dot", .traversal = {TRAV_GD,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_USER_GEN_TRAVERSALS, "dynamic_user_gen_traversals", .traversal = {TRAV_DUGT,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_DYNAMIC_USER_GEN_PASSES, "dynamic_user_gen_passes", .traversal = {TRAV_DUGP,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_STATIC_GEN_NODE_STRUCTS, "static_gen_node_structs", .traversal = {TRAV_SGNS,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_MAP_ID_TO_LIFETIME, "map_id_to_lifetime", .traversal = {TRAV_MITL,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_ASSIGN_ID_TO_ACTION, "assign_id_to_action", .traversal = {TRAV_AITA,},},
{CCN_ACTION_PASS, CCNAC_ID_DYNAMIC_GENBASENODEINIT, "dynamic_genBaseNodeInit", .pass = {PASS_DYNAMIC_GENBASENODEINIT,},},
{CCN_ACTION_PASS, CCNAC_ID_DYNAMIC_SWITCH_TO_AST_SOURCE, "dynamic_switch_to_ast_source", .pass = {PASS_DYNAMIC_SWITCH_TO_AST_SOURCE,},},
{CCN_ACTION_PASS, CCNAC_ID_DYNAMIC_GENBASENODE, "dynamic_genBaseNode", .pass = {PASS_DYNAMIC_GENBASENODE,},},
{CCN_ACTION_PASS, CCNAC_ID_DYNAMIC_START_AST_HEADER, "dynamic_start_ast_header", .pass = {PASS_DYNAMIC_START_AST_HEADER,},},
{CCN_ACTION_PASS, CCNAC_ID_DYNAMIC_SWITCH_TRAV_DATA, "dynamic_switch_trav_data", .pass = {PASS_DYNAMIC_SWITCH_TRAV_DATA,},},
{CCN_ACTION_PASS, CCNAC_ID_DYNAMIC_START_TRAV_DATA, "dynamic_start_trav_data", .pass = {PASS_DYNAMIC_START_TRAV_DATA,},},
{CCN_ACTION_PASS, CCNAC_ID_DYNAMIC_START_TABLE_GEN, "dynamic_start_table_gen", .pass = {PASS_DYNAMIC_START_TABLE_GEN,},},
{CCN_ACTION_PASS, CCNAC_ID_DYNAMIC_START_ENUM_HEADER, "dynamic_start_enum_header", .pass = {PASS_DYNAMIC_START_ENUM_HEADER,},},
{CCN_ACTION_PASS, CCNAC_ID_DOOPTS, "doOpts", .pass = {PASS_DOOPTS,},},
{CCN_ACTION_PASS, CCNAC_ID_GENDEFINES, "genDefines", .pass = {PASS_GENDEFINES,},},
{CCN_ACTION_PASS, CCNAC_ID_SCANANDPARSE, "scanAndParse", .pass = {PASS_SCANANDPARSE,},},
{CCN_ACTION_TRAVERSAL, CCNAC_ID_free, "Free", .traversal = {TRAV_free,},},
{CCN_ACTION_PHASE, CCNAC_ID_cleanup, "Cleanup", .phase = {NULL, NT_AST, cleanup_ids_table, false, CCNAC_ID_cleanup,},},
};

struct ccn_action *CCNgetActionFromID(enum ccn_action_id action_id) {
    return &ccn_action_array[action_id];
}

