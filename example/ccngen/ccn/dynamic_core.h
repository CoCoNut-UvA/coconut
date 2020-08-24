#pragma once

#include "ccngen/enum.h"
#include "ccn/ccn_types.h"


struct ccn_node *TRAVerror(struct ccn_node *arg_node);
struct ccn_node *PASSerror(struct ccn_node *arg_node);
struct ccn_node *TRAVopt(struct ccn_node *arg_node);
struct ccn_node *TRAVstart(struct ccn_node *syntaxtree, enum ccn_traversal_type trav_type);
struct ccn_node *PASSstart(struct ccn_node *syntaxtree, enum ccn_pass_type trav_type);
struct ccn_node *TRAVdo(struct ccn_node *arg_node);
struct ccn_node *TRAVchildren(struct ccn_node *arg_node);