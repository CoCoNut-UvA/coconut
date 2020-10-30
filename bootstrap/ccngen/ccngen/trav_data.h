#ifndef _CCN_TRAV_H_
#define _CCN_TRAV_H_

#include "ccn/ccn_types.h"


struct TRAV_DATA_GEN_IDS_TABLES {
    int fill;
};

#define GEN_IDS_TABLES_GEN_IDS_TABLES (trav->current()->trav_data.TD_gen_ids_tables->gen_ids_tables)
struct ccn_trav *TRAVnew_gen_ids_tables();
void TRAVfree_gen_ids_tables(struct ccn_trav *trav);
struct TRAV_DATA_GEN_ACTION_ARRAY_HEADER {
    int fill;
};

#define GEN_ACTION_ARRAY_HEADER_GEN_ACTION_ARRAY_HEADER (trav->current()->trav_data.TD_gen_action_array_header->gen_action_array_header)
struct ccn_trav *TRAVnew_gen_action_array_header();
void TRAVfree_gen_action_array_header(struct ccn_trav *trav);
struct TRAV_DATA_GEN_ACTION_ARRAY {
    int fill;
};

#define GEN_ACTION_ARRAY_GEN_ACTION_ARRAY (trav->current()->trav_data.TD_gen_action_array->gen_action_array)
struct ccn_trav *TRAVnew_gen_action_array();
void TRAVfree_gen_action_array(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENCHECKTRAVERSAL {
    int fill;
};

#define DYNAMIC_GENCHECKTRAVERSAL_DYNAMIC_GENCHECKTRAVERSAL (trav->current()->trav_data.TD_dynamic_genchecktraversal->dynamic_genCheckTraversal)
struct ccn_trav *TRAVnew_dynamic_genchecktraversal();
void TRAVfree_dynamic_genchecktraversal(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENFREETRAVERSAL {
    int fill;
};

#define DYNAMIC_GENFREETRAVERSAL_DYNAMIC_GENFREETRAVERSAL (trav->current()->trav_data.TD_dynamic_genfreetraversal->dynamic_genFreeTraversal)
struct ccn_trav *TRAVnew_dynamic_genfreetraversal();
void TRAVfree_dynamic_genfreetraversal(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENCONSTRUCTORCALL {
    int fill;
};

#define DYNAMIC_GENCONSTRUCTORCALL_DYNAMIC_GENCONSTRUCTORCALL (trav->current()->trav_data.TD_dynamic_genconstructorcall->dynamic_genConstructorCall)
struct ccn_trav *TRAVnew_dynamic_genconstructorcall();
void TRAVfree_dynamic_genconstructorcall(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENCOPYTRAVERSAL {
    int fill;
};

#define DYNAMIC_GENCOPYTRAVERSAL_DYNAMIC_GENCOPYTRAVERSAL (trav->current()->trav_data.TD_dynamic_gencopytraversal->dynamic_genCopyTraversal)
struct ccn_trav *TRAVnew_dynamic_gencopytraversal();
void TRAVfree_dynamic_gencopytraversal(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENTRAVACTIONS {
    int fill;
};

#define DYNAMIC_GENTRAVACTIONS_DYNAMIC_GENTRAVACTIONS (trav->current()->trav_data.TD_dynamic_gentravactions->dynamic_genTravActions)
struct ccn_trav *TRAVnew_dynamic_gentravactions();
void TRAVfree_dynamic_gentravactions(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENACTIONSHEADER {
    int fill;
};

#define DYNAMIC_GENACTIONSHEADER_DYNAMIC_GENACTIONSHEADER (trav->current()->trav_data.TD_dynamic_genactionsheader->dynamic_genActionsHeader)
struct ccn_trav *TRAVnew_dynamic_genactionsheader();
void TRAVfree_dynamic_genactionsheader(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENSYSTEMTRAVERSALS {
    int fill;
};

#define DYNAMIC_GENSYSTEMTRAVERSALS_DYNAMIC_GENSYSTEMTRAVERSALS (trav->current()->trav_data.TD_dynamic_gensystemtraversals->dynamic_genSystemTraversals)
struct ccn_trav *TRAVnew_dynamic_gensystemtraversals();
void TRAVfree_dynamic_gensystemtraversals(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENTRAVTABLE {
    int fill;
};

#define DYNAMIC_GENTRAVTABLE_DYNAMIC_GENTRAVTABLE (trav->current()->trav_data.TD_dynamic_gentravtable->dynamic_genTravTable)
struct ccn_trav *TRAVnew_dynamic_gentravtable();
void TRAVfree_dynamic_gentravtable(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENPASSTABLE {
    int fill;
};

#define DYNAMIC_GENPASSTABLE_DYNAMIC_GENPASSTABLE (trav->current()->trav_data.TD_dynamic_genpasstable->dynamic_genPassTable)
struct ccn_trav *TRAVnew_dynamic_genpasstable();
void TRAVfree_dynamic_genpasstable(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENNODESFORTRAVTABLE {
    int fill;
};

#define DYNAMIC_GENNODESFORTRAVTABLE_DYNAMIC_GENNODESFORTRAVTABLE (trav->current()->trav_data.TD_dynamic_gennodesfortravtable->dynamic_genNodesForTravTable)
struct ccn_trav *TRAVnew_dynamic_gennodesfortravtable();
void TRAVfree_dynamic_gennodesfortravtable(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENTRAVVTABLE {
    int fill;
};

#define DYNAMIC_GENTRAVVTABLE_DYNAMIC_GENTRAVVTABLE (trav->current()->trav_data.TD_dynamic_gentravvtable->dynamic_genTravVtable)
struct ccn_trav *TRAVnew_dynamic_gentravvtable();
void TRAVfree_dynamic_gentravvtable(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENENUMVALUES {
    int fill;
};

#define DYNAMIC_GENENUMVALUES_DYNAMIC_GENENUMVALUES (trav->current()->trav_data.TD_dynamic_genenumvalues->dynamic_genEnumValues)
struct ccn_trav *TRAVnew_dynamic_genenumvalues();
void TRAVfree_dynamic_genenumvalues(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENENUM {
    int fill;
};

#define DYNAMIC_GENENUM_DYNAMIC_GENENUM (trav->current()->trav_data.TD_dynamic_genenum->dynamic_genEnum)
struct ccn_trav *TRAVnew_dynamic_genenum();
void TRAVfree_dynamic_genenum(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENNODECONSTRUCTOR {
    int fill;
};

#define DYNAMIC_GENNODECONSTRUCTOR_DYNAMIC_GENNODECONSTRUCTOR (trav->current()->trav_data.TD_dynamic_gennodeconstructor->dynamic_genNodeConstructor)
struct ccn_trav *TRAVnew_dynamic_gennodeconstructor();
void TRAVfree_dynamic_gennodeconstructor(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENNODEUNION {
    int fill;
};

#define DYNAMIC_GENNODEUNION_DYNAMIC_GENNODEUNION (trav->current()->trav_data.TD_dynamic_gennodeunion->dynamic_genNodeUnion)
struct ccn_trav *TRAVnew_dynamic_gennodeunion();
void TRAVfree_dynamic_gennodeunion(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENNODEINITFUNCTIONS {
    int fill;
};

#define DYNAMIC_GENNODEINITFUNCTIONS_DYNAMIC_GENNODEINITFUNCTIONS (trav->current()->trav_data.TD_dynamic_gennodeinitfunctions->dynamic_genNodeInitFunctions)
struct ccn_trav *TRAVnew_dynamic_gennodeinitfunctions();
void TRAVfree_dynamic_gennodeinitfunctions(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENNODESTRUCTS {
    int fill;
};

#define DYNAMIC_GENNODESTRUCTS_DYNAMIC_GENNODESTRUCTS (trav->current()->trav_data.TD_dynamic_gennodestructs->dynamic_genNodeStructs)
struct ccn_trav *TRAVnew_dynamic_gennodestructs();
void TRAVfree_dynamic_gennodestructs(struct ccn_trav *trav);
struct TRAV_DATA_DYNAMIC_GENNODEMACROS {
    int fill;
};

#define DYNAMIC_GENNODEMACROS_DYNAMIC_GENNODEMACROS (trav->current()->trav_data.TD_dynamic_gennodemacros->dynamic_genNodeMacros)
struct ccn_trav *TRAVnew_dynamic_gennodemacros();
void TRAVfree_dynamic_gennodemacros(struct ccn_trav *trav);
struct TRAV_DATA_CHECKEXISTANCE {
    int fill;
};

#define CHECKEXISTANCE_CHECKEXISTANCE (trav->current()->trav_data.TD_checkexistance->checkExistance)
struct ccn_trav *TRAVnew_checkexistance();
void TRAVfree_checkexistance(struct ccn_trav *trav);
struct TRAV_DATA_BUILDSYMBOLTABLE {
    int fill;
};

#define BUILDSYMBOLTABLE_BUILDSYMBOLTABLE (trav->current()->trav_data.TD_buildsymboltable->buildSymbolTable)
struct ccn_trav *TRAVnew_buildsymboltable();
void TRAVfree_buildsymboltable(struct ccn_trav *trav);
struct TRAV_DATA_UNWRAPSETEXPR {
    int fill;
};

#define UNWRAPSETEXPR_UNWRAPSETEXPR (trav->current()->trav_data.TD_unwrapsetexpr->unwrapSetExpr)
struct ccn_trav *TRAVnew_unwrapsetexpr();
void TRAVfree_unwrapsetexpr(struct ccn_trav *trav);
struct TRAV_DATA_PRINT {
    int indent;
};

#define PRINT_PRINT (trav->current()->trav_data.TD_print->print)
struct ccn_trav *TRAVnew_print();
void TRAVfree_print(struct ccn_trav *trav);
union TRAV_DATA {
    struct TRAV_DATA_GEN_IDS_TABLES *TD_gen_ids_tables;
    struct TRAV_DATA_GEN_ACTION_ARRAY_HEADER *TD_gen_action_array_header;
    struct TRAV_DATA_GEN_ACTION_ARRAY *TD_gen_action_array;
    struct TRAV_DATA_DYNAMIC_GENCHECKTRAVERSAL *TD_dynamic_genchecktraversal;
    struct TRAV_DATA_DYNAMIC_GENFREETRAVERSAL *TD_dynamic_genfreetraversal;
    struct TRAV_DATA_DYNAMIC_GENCONSTRUCTORCALL *TD_dynamic_genconstructorcall;
    struct TRAV_DATA_DYNAMIC_GENCOPYTRAVERSAL *TD_dynamic_gencopytraversal;
    struct TRAV_DATA_DYNAMIC_GENTRAVACTIONS *TD_dynamic_gentravactions;
    struct TRAV_DATA_DYNAMIC_GENACTIONSHEADER *TD_dynamic_genactionsheader;
    struct TRAV_DATA_DYNAMIC_GENSYSTEMTRAVERSALS *TD_dynamic_gensystemtraversals;
    struct TRAV_DATA_DYNAMIC_GENTRAVTABLE *TD_dynamic_gentravtable;
    struct TRAV_DATA_DYNAMIC_GENPASSTABLE *TD_dynamic_genpasstable;
    struct TRAV_DATA_DYNAMIC_GENNODESFORTRAVTABLE *TD_dynamic_gennodesfortravtable;
    struct TRAV_DATA_DYNAMIC_GENTRAVVTABLE *TD_dynamic_gentravvtable;
    struct TRAV_DATA_DYNAMIC_GENENUMVALUES *TD_dynamic_genenumvalues;
    struct TRAV_DATA_DYNAMIC_GENENUM *TD_dynamic_genenum;
    struct TRAV_DATA_DYNAMIC_GENNODECONSTRUCTOR *TD_dynamic_gennodeconstructor;
    struct TRAV_DATA_DYNAMIC_GENNODEUNION *TD_dynamic_gennodeunion;
    struct TRAV_DATA_DYNAMIC_GENNODEINITFUNCTIONS *TD_dynamic_gennodeinitfunctions;
    struct TRAV_DATA_DYNAMIC_GENNODESTRUCTS *TD_dynamic_gennodestructs;
    struct TRAV_DATA_DYNAMIC_GENNODEMACROS *TD_dynamic_gennodemacros;
    struct TRAV_DATA_CHECKEXISTANCE *TD_checkexistance;
    struct TRAV_DATA_BUILDSYMBOLTABLE *TD_buildsymboltable;
    struct TRAV_DATA_UNWRAPSETEXPR *TD_unwrapsetexpr;
    struct TRAV_DATA_PRINT *TD_print;
};


#endif /* _CCN_TRAV_H_ */
