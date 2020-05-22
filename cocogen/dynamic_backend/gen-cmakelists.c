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
    out("        enum.h\n");
    out("        trav.h\n");
    out(")\n");
}

void gen_source_cmakelists(Config *config, FILE *fp) {
    out("target_sources(${PROJECT_NAME}\n");
    out("    PRIVATE\n");
    out("        ast.c\n");
    out("        trav.c\n");
    out("        actions.c\n");
    out("        vtables.c\n");
    out("        trav_copy.c\n");
    out("        trav_free.c\n");
    out("        trav_check.c\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        out("        trav_%s.c\n", trav->id->lwr);
    }
    for (int i = 0; i < array_size(config->passes); i++) {
        Pass *pass = array_get(config->passes, i);
        char *passid = NULL;
        if (pass->func) {
            passid = pass->func->lwr;
        } else {
            passid = pass->id->lwr;
        }
        out("        pass_%s.c\n", passid);
    }
    out(")\n");
}