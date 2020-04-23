#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/formatting.h"
#include "filegen/gen-util.h"
#include "filegen/genmacros.h"
#include "lib/smap.h"

static int indent = 0;

void gen_globaldata_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_GLOBALDATA_H_\n");
    out("#define _CCN_GLOBALDATA_H_\n\n");
    out("#include \"generated/enum.h\"\n");
    out("\n");
    out_field("void *globaldata[_TRAV_SIZE]");
    out("\n");
    out_field("void init_globaldata()");
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travupr = strupr(trav->id);
        char *travlwr = strlwr(trav->id);
        out_field("struct %s_DATA", travupr);
        out_field("typedef struct %s_DATA %sData", travupr, trav->id);
        out_field("%sData *%s_init_data()", trav->id, travlwr);
        out_field("void %s_free_data()", travlwr);
        out("#define GET_%s_DATA(item) ((%sData *)globaldata[TRAV_%s])->item\n",
            travupr, trav->id, travlwr);
        free(travlwr);
        free(travupr);
        out("\n");
    }
    out_field("struct FREE_DATA");
    out_field("typedef struct FREE_DATA FreeData");
    out_field("FreeData *free_init_data()");
    out_field("void free_free_data()");
    out("#define GET_FREE_DATA(item) ((FreeData "
        "*)globaldata[TRAV_free])->item\n");
    out("\n");
    out_field("struct COPY_DATA");
    out_field("typedef struct COPY_DATA CopyData");
    out_field("CopyData *copy_init_data()");
    out_field("void copy_free_data()");
    out("#define GET_COPY_DATA(item) ((CopyData "
        "*)globaldata[TRAV_copy])->item\n");
    out("\n");
    out("#endif /* _CCN_GLOBALDATA_H_ */\n");
}

void gen_globaldata_src(Config *config, FILE *fp) {
    out("#include \"generated/globaldata.h\"\n");
    out("\n");
    out_start_func("void init_globaldata()");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out_field("globaldata[TRAV_%s] = %s_init_data()", travlwr, travlwr);
        free(travlwr);
    }
    out_field("globaldata[TRAV_free] = free_init_data()");
    out_field("globaldata[TRAV_copy] = copy_init_data()");
    out_end_func();
}