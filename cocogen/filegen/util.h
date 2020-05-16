#pragma once

#include "ast/ast.h"
#include <stdio.h>

void print_indent_level(int indent_level, FILE *);
void generate_node_header_includes(Config *, FILE *, Node *);

char *strupr(char *string);
char *strlwr(char *string);

bool is_traversal_node(Traversal *trav, Node *node);

#include "genmacros.h"