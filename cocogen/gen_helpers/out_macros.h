#pragma once


#pragma once

#include <stdio.h>
#include "generator/generator.h"
#include "gen_helpers/format.h"


#define OUT(fmt, ...) GNprint(ctx, GN_DEFAULT, fmt, ## __VA_ARGS__)
#define OUT_NO_INDENT(fmt, ...) GNprint(ctx, GN_NO_WS, fmt, ## __VA_ARGS__)
#define OUT_STRUCT(fmt, ...) GNprint(ctx, GN_INCREASE_WS_AFTER, "struct " fmt " {\n", ## __VA_ARGS__)
#define OUT_TYPEDEF_STRUCT(fmt, ...) GNprint(ctx, GN_INCREASE_WS_AFTER, "typedef struct " fmt " {\n", ## __VA_ARGS__)
#define OUT_UNION(fmt, ...) GNprint(ctx, GN_INCREASE_WS_AFTER, "union " fmt " {\n", ## __VA_ARGS__)
#define OUT_FIELD(fmt, ...) GNprint(ctx, GN_DEFAULT, fmt ";\n", ## __VA_ARGS__)
#define OUT_STRUCT_END() GNprint(ctx, GN_DECREASE_WS_BEFORE, "};\n\n")
#define OUT_TYPEDEF_STRUCT_END(fmt, ...)  GNprint(ctx, GN_DECREASE_WS_BEFORE, "} " fmt ";\n\n", ##__VA_ARGS__)
#define OUT_STATEMENT(fmt, ...) GNprint(ctx, GN_DEFAULT, fmt ";\n", ## __VA_ARGS__)
#define OUT_START_FUNC(fmt, ...) GNprint(ctx, GN_INCREASE_WS_AFTER, fmt " {\n", ## __VA_ARGS__)
#define OUT_END_FUNC() GNprint(ctx, GN_DECREASE_WS_BEFORE, "}\n\n")
#define OUT_START_FUNC_FIELD() GNprint(ctx, GN_INCREASE_WS_AFTER, " {\n")
#define OUT_BEGIN_IF(fmt, ...) GNprint(ctx, GN_INCREASE_WS_AFTER, "if ("fmt ") {\n", ## __VA_ARGS__)
#define OUT_END_IF() GNprint(ctx, GN_DECREASE_WS_BEFORE, "}\n\n")
#define OUT_ENUM_FIELD(fmt, ...)  GNprint(ctx, GN_DEFAULT, fmt ",\n", ## __VA_ARGS__)
#define OUT_BEGIN_SWITCH(fmt, ...) GNprint(ctx, GN_INCREASE_WS_AFTER, "switch (" fmt ") {\n", ## __VA_ARGS__)
#define OUT_BEGIN_CASE(fmt, ...) GNprint(ctx, GN_INCREASE_WS_AFTER, "case " fmt ":\n", ## __VA_ARGS__)
#define OUT_END_CASE() GNprint(ctx, GN_DECREASE_WS_AFTER, "break;\n");
#define OUT_END_CASE_NO_BREAK() GNprint(ctx, GN_DECREASE_WS_AFTER, "");
#define OUT_BEGIN_DEFAULT_CASE() GNprint(ctx, GN_INCREASE_WS_AFTER, "default:\n")
#define OUT_END_SWITCH() GNprint(ctx, GN_DECREASE_WS_BEFORE, "}\n\n")
#define OUT_ENUM(fmt, ...) GNprint(ctx, GN_INCREASE_WS_AFTER, "enum " fmt " {\n", ##__VA_ARGS__)
#define OUT_ENUM_END(...)  GNprint(ctx, GN_DECREASE_WS_BEFORE, "};\n\n")

