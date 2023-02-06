//
// Created by df on 20/09/2021.
//

#include "generator.h"
#include "palm/memory.h"
#include "palm/str.h"
#include <assert.h>
#include <globals.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct GeneratorContext {
    FILE *fp;
    unsigned short indent_level;
    unsigned short indent_size;
} GeneratorContext;


GeneratorContext *GNnew()
{
    GeneratorContext *ctx = MEMmalloc(sizeof(GeneratorContext));
    ctx->indent_size = 4;
    ctx->indent_level = 0;
    return ctx;
}

void GNindentIncrease(GeneratorContext *ctx)
{
    ctx->indent_level++;
}

void GNindentDecrease(GeneratorContext *ctx)
{
    assert(ctx->indent_level > 0);
    ctx->indent_level--;
}

void GNopenFile(GeneratorContext *ctx, const char *filename)
{
    printf("Opening: %s\n", filename);
    if (ctx->fp) {
        fclose(ctx->fp);
    }
    ctx->fp = fopen(filename, "w");
    if (!ctx->fp) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        exit(1);
    }
}

void GNopenIncludeFile(GeneratorContext *ctx, const char *filename)
{
    char *full_file = STRcat(globals.gen_hdr_dir, filename);
    GNopenFile(ctx, full_file);
    MEMfree(full_file);

    GNprint(ctx, GN_DEFAULT, "#pragma once\n");
}

void GNopenSourceFile(GeneratorContext *ctx, const char *filename)
{
    char *full_file = STRcat(globals.gen_src_dir, filename);
    GNopenFile(ctx, full_file);
    MEMfree(full_file);
}

void GNopenUserFile(GeneratorContext *ctx, const char *filename)
{
    char *full_file = STRcat(globals.gen_user_dir, filename);
    GNopenFile(ctx, full_file);
    MEMfree(full_file);
}

void GNprint(GeneratorContext *ctx, unsigned int style, const char *fmt, ...)
{
    if (style & GN_INCREASE_WS_BEFORE) {
        GNindentIncrease(ctx);
    }
    if (style & GN_DECREASE_WS_BEFORE) {
        GNindentDecrease(ctx);
    }

    va_list format_args;
    va_start(format_args, fmt);

    if (!(style & GN_NO_WS) && ctx->indent_level > 0) {
        fprintf(ctx->fp, "%*c", ctx->indent_level * ctx->indent_size, ' ');
    }

    vfprintf(ctx->fp, fmt, format_args);
    va_end(format_args);

    if (style & GN_INCREASE_WS_AFTER) {
        GNindentIncrease(ctx);
    }
    if (style & GN_DECREASE_WS_AFTER) {
        GNindentDecrease(ctx);
    }
}
