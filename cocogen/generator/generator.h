//
// Created by df on 20/09/2021.
//

#pragma once


#define GN_DEFAULT 0
#define GN_INCREASE_WS_AFTER (1 << 0)
#define GN_INCREASE_WS_BEFORE (1 << 1)
#define GN_NO_WS (1 << 2)
#define GN_DECREASE_WS_AFTER (1 << 3)
#define GN_DECREASE_WS_BEFORE (1 << 4)


typedef struct GeneratorContext GeneratorContext;
GeneratorContext *GNnew();
void GNprint(GeneratorContext *ctx, unsigned int style, const char *fmt, ...);
void GNopenIncludeFile(GeneratorContext *ctx, const char *filename);
void GNopenSourceFile(GeneratorContext *ctx, const char *filename);
void GNopenUserFile(GeneratorContext *ctx, const char *filename);
void GNopenFile(GeneratorContext *ctx, const char *filename);
void GNindentIncrease(GeneratorContext *ctx);
void GNindentDecrease(GeneratorContext *ctx);
