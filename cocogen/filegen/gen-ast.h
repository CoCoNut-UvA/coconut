#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast/ast.h"

void gen_ast_header(Config *config, FILE *fp);
void gen_ast_src(Config *config, FILE *fp);