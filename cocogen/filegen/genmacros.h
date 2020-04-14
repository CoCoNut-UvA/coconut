#pragma once

#include "ast/ast.h"
#include "filegen/formatting.h"
#include <stdio.h>

#define COLOR_GREEN "\033[1m\033[32m"
#define COLOR_RESET "\033[0m"
#define out(...) fprintf(fp, __VA_ARGS__)
#define out_struct(...)                                                        \
    print_indent_level(indent, fp);                                            \
    indent++;                                                                  \
    fprintf(fp, "struct ");                                                    \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")
#define out_union(...)                                                         \
    print_indent_level(indent, fp);                                            \
    indent++;                                                                  \
    fprintf(fp, "union ");                                                     \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")
#define out_field(...)                                                         \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ";\n")
#define out_struct_end()                                                       \
    indent--;                                                                  \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "};\n\n")
#define out_statement(...)                                                     \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ";\n")
#define out_start_func(...)                                                    \
    print_indent_level(indent, fp);                                            \
    indent++;                                                                  \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")
#define out_end_func()                                                         \
    indent--;                                                                  \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "}\n\n")
#define out_begin_if(...)                                                      \
    print_indent_level(indent, fp);                                            \
    indent++;                                                                  \
    fprintf(fp, "if (");                                                       \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ") {\n")
#define out_end_if()                                                           \
    indent--;                                                                  \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "}\n\n");
#define out_begin_for(...)                                                     \
    print_indent_level(indent, fp);                                            \
    indent++;                                                                  \
    fprintf(fp, "for (");                                                      \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ") {\n");
#define out_end_for()                                                          \
    indent--;                                                                  \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "}\n");
#define out_begin_while(...)                                                   \
    print_indent_level(indent, fp);                                            \
    indent++;                                                                  \
    fprintf(fp, "while (");                                                    \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ") {\n");
#define out_end_while()                                                        \
    indent--;                                                                  \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "}\n");
#define out_enum_field(...)                                                    \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ",\n");
#define out_begin_else()                                                       \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "else {\n");                                                   \
    indent++;
#define out_end_else()                                                         \
    indent--;                                                                  \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "}\n\n");
#define out_begin_switch(...)                                                  \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "switch (");                                                   \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ") {\n");
#define out_begin_case(...)                                                    \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "case ");                                                      \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ":\n");                                                        \
    indent++;
#define out_end_case() indent--;
#define out_begin_default_case()                                               \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "default:\n");                                                 \
    indent++;
#define out_end_switch()                                                       \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "}\n");
#define out_comment(...)                                                       \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "// ");                                                        \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, "\n");
#define out_enum(...)                                                          \
    print_indent_level(indent, fp);                                            \
    indent++;                                                                  \
    fprintf(fp, "typedef enum ");                                              \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")
#define out_enum_end(...)                                                      \
    indent--;                                                                  \
    print_indent_level(indent, fp);                                            \
    fprintf(fp, "} ");                                                         \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ";\n\n");
void print_indent_level(int indent_level, FILE *);
void generate_node_header_includes(Config *, FILE *, Node *);
