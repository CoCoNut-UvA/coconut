#pragma once


#pragma once

#include <stdio.h>

#include "gen_helpers/format.h"
#include "ccngen/ast.h"


#define OUT(...) FMTprintIndentLevel(fp, indent); fprintf(fp, __VA_ARGS__)

#define OUT_NO_INDENT(...) (fprintf(fp, __VA_ARGS__))

#define OUT_STRUCT(...)                                                        \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, "struct ");                                                    \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")

#define OUT_TYPEDEF_STRUCT(...)                                                \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, "typedef struct ");                                            \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")

#define OUT_UNION(...)                                                         \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, "union ");                                                     \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")

#define OUT_FIELD(...)                                                         \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ";\n")

#define OUT_STRUCT_END()                                                       \
    indent--;                                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "};\n\n")

#define OUT_TYPEDEF_STRUCT_END(...)                                            \
    indent--;                                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "} ");                                                         \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ";\n\n")

#define OUT_STATEMENT(...)                                                     \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ";\n")

#define OUT_START_FUNC(...)                                                    \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")

#define OUT_START_FUNC_FIELD()                                                 \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, " {\n")

#define OUT_END_FUNC()                                                         \
    indent--;                                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "}\n\n")

#define OUT_BEGIN_IF(...)                                                      \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, "if (");                                                       \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ") {\n")

#define OUT_END_IF()                                                           \
    indent--;                                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "}\n\n");

#define OUT_BEGIN_FOR(...)                                                     \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, "for (");                                                      \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ") {\n");

#define OUT_END_FOR()                                                          \
    indent--;                                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "}\n");

#define OUT_BEGIN_WHILE(...)                                                   \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, "while (");                                                    \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ") {\n");

#define OUT_END_WHILE()                                                        \
    indent--;                                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "}\n");

#define OUT_ENUM_FIELD(...)                                                    \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ",\n");

#define OUT_BEGIN_ELSE()                                                       \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "else {\n");                                                   \
    indent++;

#define OUT_END_ELSE()                                                         \
    indent--;                                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "}\n\n");

#define OUT_BEGIN_SWITCH(...)                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "switch (");                                                   \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ") {\n");

#define OUT_BEGIN_CASE(...)                                                    \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "case ");                                                      \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, ":\n");                                                        \
    indent++;

#define OUT_END_CASE() indent--;

#define OUT_BEGIN_DEFAULT_CASE()                                               \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "default:\n");                                                 \
    indent++;

#define OUT_END_SWITCH()                                                       \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "}\n");

#define OUT_COMMENT(...)                                                       \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "// ");                                                        \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, "\n");

#define OUT_ENUM(...)                                                          \
    FMTprintIndentLevel(fp, indent);                                            \
    indent++;                                                                  \
    fprintf(fp, "enum ");                                              \
    fprintf(fp, __VA_ARGS__);                                                  \
    fprintf(fp, " {\n")

#define OUT_ENUM_END(...)                                                      \
    indent--;                                                                  \
    FMTprintIndentLevel(fp, indent);                                            \
    fprintf(fp, "};\n\n ");
