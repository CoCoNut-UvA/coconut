#pragma once
#include "ccn/ccn_types.h"
#include "ccn/dynamic_core.h"

struct ccn_node *changebinop(struct ccn_node *node);
struct ccn_node *changenum(struct ccn_node *node);
struct ccn_node *travbinop(struct ccn_node *node);
struct ccn_node *travnum(struct ccn_node *node);
struct ccn_node *CPYbinop(struct ccn_node *arg_node);
struct ccn_node *DELbinop(struct ccn_node *arg_node);
struct ccn_node *CHKbinop(struct ccn_node *arg_node);
struct ccn_node *CPYnum(struct ccn_node *arg_node);
struct ccn_node *DELnum(struct ccn_node *arg_node);
struct ccn_node *CHKnum(struct ccn_node *arg_node);
struct ccn_node *p(struct ccn_node *);
