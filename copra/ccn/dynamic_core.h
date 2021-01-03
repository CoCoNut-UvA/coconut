#pragma once

#include "ccngen/enum.h"
#include "ccn/ccn_types.h"
#include "ccngen/trav_data.h"

typedef struct ccn_trav {
    struct ccn_trav *prev;
    enum ccn_traversal_type trav_type;
    union TRAV_DATA trav_data;
} ccn_trav_st;

struct ccn_node *TRAVerror(struct ccn_node *arg_node);
struct ccn_node *PASSerror(struct ccn_node *arg_node);
struct ccn_node *TRAVopt(struct ccn_node *arg_node);
struct ccn_node *TRAVstart(struct ccn_node *syntaxtree, enum ccn_traversal_type trav_type);
struct ccn_node *PASSstart(struct ccn_node *syntaxtree, enum ccn_pass_type trav_type);
struct ccn_node *TRAVdo(struct ccn_node *arg_node);
void TRAVpush(enum ccn_traversal_type trav_type);
void TRAVpop();
struct ccn_node *TRAVchildren(struct ccn_node *arg_node);
struct ccn_node *TRAVnop(struct ccn_node *arg_node);
void TRAVdataNOP(ccn_trav_st *);
ccn_trav_st *TRAVgetCurrent(void);
struct ccn_node *CCNcopy(struct ccn_node *arg_node);
struct ccn_node *CCNfree(struct ccn_node *arg_node);
