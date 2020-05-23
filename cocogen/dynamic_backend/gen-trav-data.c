#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/util.h"
#include "filegen/driver.h"
#include "filegen/formatting.h"
#include "filegen/genmacros.h"
#include "filegen/reachability.h"
#include "filegen/util.h"
#include "gen-functions.h"
#include "lib/print.h"
#include "lib/smap.h"

static int indent = 0;

void gen_trav_data_struct(Config *config, FILE *fp, Traversal *trav) {
    if (!trav->data) {
        return;
    }
    out_comment("Traversal %s Attributes", trav->id->orig);
    out_struct("TRAV_DATA_%s", trav->id->upr);
    for (int i = 0; i < array_size(trav->data); ++i) {
        Attr *td = (Attr *)array_get(trav->data, i);
        char *type_str = str_attr_type(td);
        if (td->type == AT_link_or_enum) {
            out_field("%s *%s", type_str, td->id->orig);
        } else {
            out_field("%s %s", type_str, td->id->orig);
        }
    }
    out_struct_end();
}

void gen_trav_data_union(Config *config, FILE *fp) {
    out_comment("Attributes");
    out_union("TRAV_DATA");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = (Traversal *)array_get(config->traversals, i);
        if (!trav->data) {
            continue;
        }
        out_field("struct TRAV_DATA_%s *TD_%s", trav->id->upr, trav->id->lwr);
    }
    out_struct_end();
}

void gen_trav_data_macros(Config *config, FILE *fp, Traversal *trav) {
    if (!trav->data) {
        return;
    }
    out_comment("Macros for Traversal %s", trav->id->orig);
    for (int i = 0; i < array_size(trav->data); ++i) {
        Attr *td = (Attr *)array_get(trav->data, i);
        out("#define %s_%s (trav_current()->trav_data.TD_%s->%s)\n",
            trav->id->upr, td->id->upr, trav->id->lwr, td->id->orig);
    }
}

void gen_trav_data_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_TRAV_H_\n");
    out("#define _CCN_TRAV_H_\n\n");
    out("#include \"../copra/include/types.h\"\n");
    out("\n");

    out_comment("Includes for user defined types");
    smap_t *include_names = smap_init(16);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        for (int i = 0; i < array_size(trav->data); i++) {
            Attr *td = array_get(trav->data, i);
            if (td->type == AT_link_or_enum) {
                char *include = td->include;
                if (!smap_retrieve(include_names, include)) {
                    smap_insert(include_names, include, td);
                    out("#include \"%s\"\n", include);
                }
            }
        }
    }
    smap_free(include_names);
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_data_struct(config, fp, trav);
    }
    gen_trav_data_union(config, fp);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        if (trav->data) {
            out_field("Trav *trav_init_%s()", trav->id->lwr);
            out_field("void trav_free_%s(Trav *trav)", trav->id->lwr);
        }
    }
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_data_macros(config, fp, trav);
    }
    out("\n");
    out("#endif /* _CCN_TRAV_H_ */\n");
}

void gen_trav_data_constructor(Config *config, FILE *fp, Traversal *trav) {
    out_start_func("Trav *trav_init_%s(Trav *trav)", trav->id->lwr);
    out_field("trav->trav_data.TD_%s = mem_alloc(sizeof(struct TRAV_DATA_%s))",
              trav->id->lwr, trav->id->upr);
    out_field("%s_init_trav_data(trav)", trav->id->lwr);
    out_field("return trav");
    out_end_func();
}

void gen_trav_data_destructor(Config *config, FILE *fp, Traversal *trav) {
    out_start_func("void trav_free_%s(Trav *trav)", trav->id->lwr);
    out_field("%s_free_trav_data(trav)", trav->id->lwr);
    out_field("mem_free(trav->trav_data.TD_%s)", trav->id->lwr);
    out_end_func();
}

void gen_trav_data_src(Config *config, FILE *fp) {
    out("#include <stdlib.h>\n");
    out("\n");
    out("#include \"../copra/include/actions_core.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        if (trav->data) {
            gen_trav_data_constructor(config, fp, trav);
            gen_trav_data_destructor(config, fp, trav);
        }
    }
}
