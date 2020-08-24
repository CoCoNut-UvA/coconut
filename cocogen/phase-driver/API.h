#pragma once
#include "ast/ast.h"
#include <stdio.h>

extern void gen_action_array_h(Config *c, FILE *fp);
extern void gen_action_array_c(Config *c, FILE *fp);