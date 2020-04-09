#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast/ast.h"

void gen_enum_header(Config *config, FILE *fp);
void gen_enum_src(Config *config, FILE *fp);