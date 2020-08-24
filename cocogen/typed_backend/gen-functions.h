#pragma once

#include <stdio.h>
#include "ast/ast.h"

extern void generate_pass_header(Config *config, FILE *fp, Pass *pass);
extern void generate_enum_definitions(Config *config, FILE *fp);
extern void generate_ast_definitions(Config *config, FILE *fp);
extern void generate_ast_node_header(Config *, FILE *, Node *);
extern void generate_ast_nodeset_header(Config *, FILE *, Nodeset *);

extern void generate_copy_node_header(Config *c, FILE *fp, Node *n);
extern void generate_copy_node_definitions(Config *c, FILE *fp, Node *n);
extern void generate_copy_nodeset_header(Config *c, FILE *fp, Nodeset *n);
extern void generate_copy_nodeset_definitions(Config *c, FILE *fp, Nodeset *n);
extern void generate_copy_header(Config *config, FILE *fp);

extern void generate_create_node_header(Config *c, FILE *fp, Node *n);
extern void generate_create_node_definitions(Config *c, FILE *fp, Node *n);
extern void generate_create_nodeset_header(Config *c, FILE *fp, Nodeset *n);
extern void generate_create_nodeset_definitions(Config *c, FILE *fp, Nodeset *n);
extern void generate_create_header(Config *config, FILE *fp);

extern void generate_free_header(Config *, FILE *);
extern void generate_free_node_header(Config *c, FILE *fp, Node *n);
extern void generate_free_node_definitions(Config *c, FILE *fp, Node *n);
extern void generate_free_nodeset_header(Config *c, FILE *fp, Nodeset *n);
extern void generate_free_nodeset_definitions(Config *c, FILE *fp, Nodeset *n);

extern void generate_user_trav_header(Config *config, FILE *fp, Traversal *trav);

extern void generate_gate_headers(Config *config, FILE *fp);

extern void generate_trav_header(Config *, FILE *);
extern void generate_trav_node_header(Config *, FILE *, Node *);
extern void generate_trav_node_definitions(Config *, FILE *, Node *);

extern void generate_trav_core_header(Config *config, FILE *fp);
extern void generate_trav_core_definitions(Config *config, FILE *fp);

extern void gen_action_array_h(Config *c, FILE *fp);
extern void gen_action_array_c(Config *c, FILE *fp);

extern void subtree_generate_set_handler(char *root, ccn_set_t *funcs);
extern void subtree_generate_find_traversal_body(char *trav_name, char *target);
extern void subtree_generate_phase_functions(Config *config, array *phases);
extern void subtree_generate_traversals(Config *config);
extern void subtree_generate_call_find_sub_root(char *root, char *to_find, FILE *fp, Phase *phase, int *_indent);