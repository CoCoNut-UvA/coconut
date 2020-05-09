#pragma once

#include "ast/ast.h"
#include <stdio.h>

void gen_actions_header(Config *config, FILE *fp);
void gen_actions_src(Config *config, FILE *fp);
void gen_ast_header(Config *config, FILE *fp);
void gen_ast_src(Config *config, FILE *fp);
void gen_header_cmakelists(Config *config, FILE *fp);
void gen_source_cmakelists(Config *config, FILE *fp);
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