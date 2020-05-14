#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

void gen_header_cmakelists(Config *config, FILE *fp) {
    out("target_sources(${PROJECT_NAME}\n");
    out("    PUBLIC\n");
    out("        ast.h\n");
    out("        trav.h\n");
    out("        actions.h\n");
    out("        copy.h\n");
    out("        enum.h\n");
    out("        free.h\n");
    out("        trav.h\n");
    out(")\n");
}

void gen_source_cmakelists(Config *config, FILE *fp) {
    out("target_sources(${PROJECT_NAME}\n");
    out("    PRIVATE\n");
    out("        ast.c\n");
    out("        trav.c\n");
    out("        actions.c\n");
    out("        copy.c\n");
    out("        free.c\n");
    out("        vtables.c\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("        trav_%s.c\n", travlwr);
        mem_free(travlwr);
    }
    for (int i = 0; i < array_size(config->passes); i++) {
        Pass *pass = array_get(config->passes, i);
        char *passlwr;
        if (pass->func) {
            passlwr = strlwr(pass->func);
        } else {
            passlwr = strlwr(pass->id);
        }
        out("        pass_%s.c\n", passlwr);
        mem_free(passlwr);
    }
    out(")\n");
}