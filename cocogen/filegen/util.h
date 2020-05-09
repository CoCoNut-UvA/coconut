#pragma once

#include "ast/ast.h"
#include <stdio.h>

void print_indent_level(int indent_level, FILE *);
void generate_node_header_includes(Config *, FILE *, Node *);

#include "genmacros.h"