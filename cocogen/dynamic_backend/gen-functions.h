#pragma once

#include "ast/ast.h"
#include <stdio.h>

void gen_actions_header(Config *config, FILE *fp);
void gen_actions_src(Config *config, FILE *fp);
void gen_ast_header(Config *config, FILE *fp);
void gen_ast_src(Config *config, FILE *fp);
void gen_generated_cmakelists(Config *config, FILE *fp);
void gen_user_cmakelists(Config *config, FILE *fp);
void gen_copy_header(Config *config, FILE *fp);
void gen_copy_src(Config *config, FILE *fp);
void gen_enum_header(Config *config, FILE *fp);
void gen_enum_src(Config *config, FILE *fp);
void gen_free_header(Config *config, FILE *fp);
void gen_free_src(Config *config, FILE *fp);
void gen_trav_user_header(Config *config, FILE *fp, Traversal *trav);
void gen_trav_user_src(Config *config, FILE *fp, Traversal *trav);
void gen_trav_header(Config *config, FILE *fp);
void gen_trav_src(Config *config, FILE *fp);

char *strupr(char *string);
char *strlwr(char *string);
bool is_traversal_node(Traversal *trav, Node *node);

static smap_t *node_index;
static smap_t *trav_index;

// Node reachability matrix
static bool **node_reachability;

static bool **pass_nodes;

void compute_reachable_nodes(Config *config);
int get_trav_index(char *id);
int get_node_index(char *id);
bool is_pass_node(Traversal *trav, Node *node);