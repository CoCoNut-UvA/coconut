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
