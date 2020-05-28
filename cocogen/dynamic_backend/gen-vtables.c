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
#include "lib/str.h"

static int indent = 0;

void gen_system_vtable(Config *config, FILE *fp, char *version) {
    char *verlwr = ccn_str_lwr(version);
    out("const TravFunc %s_vtable[_NT_SIZE] = { &trav_error, ", verlwr);
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        out("&%s_%s, ", verlwr, node->id->lwr);
    }
    out(" };\n\n");
    mem_free(verlwr);
}

void gen_error_vtable(Config *config, FILE *fp) {
    out("const TravFunc error_vtable[_NT_SIZE] = { &trav_error, ");
    for (int j = 0; j < array_size(config->nodes); j++) {
        out("&trav_error, ");
    }
    out(" };\n\n");
}

void gen_trav_vtable(Config *config, FILE *fp, Traversal *trav) {
    out("const TravFunc %s_vtable[_NT_SIZE] = { &trav_error, ", trav->id->lwr);
    int tindex = get_trav_index(trav->id->orig);
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        if (is_traversal_node(trav, node)) {
            out("&%s_%s, ", trav->id->lwr, node->id->lwr);
        } else if (is_pass_node(trav, node)) {
            out("&trav_children, ");
        } else {
            out("&trav_return, ");
        }
    }
    out(" };\n\n");
}

void gen_vtables(Config *config, FILE *fp) {
    gen_error_vtable(config, fp);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        gen_trav_vtable(config, fp, trav);
    }
    gen_system_vtable(config, fp, "free");
    gen_system_vtable(config, fp, "copy");
    gen_system_vtable(config, fp, "check");
    out("const TravFunc *trav_vtable[_TRAV_SIZE] = { error_vtable, ");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        out("%s_vtable, ", trav->id->lwr);
    }
    out("free_vtable, ");
    out("copy_vtable, ");
    out("check_vtable, ");
    out(" };\n\n");
}

void gen_trav_data_vtable(Config *config, FILE *fp, char *version) {
    char *verlwr = ccn_str_lwr(version);
    out("const %sFunc trav_data_%s_vtable[_TRAV_SIZE] = { ", version, verlwr);
    out("&trav_%s_error, ", verlwr);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        if (trav->data) {
            out("&trav_%s_%s, ", verlwr, trav->id->lwr);
        } else {
            out("&trav_%s_return, ", verlwr);
        }
    }
    out("&trav_%s_return, ", verlwr); // Free
    out("&trav_%s_return, ", verlwr); // Copy
    out("&trav_%s_return, ", verlwr); // Check
    out(" };\n\n");
    mem_free(verlwr);
}

void gen_pass_vtable(Config *config, FILE *fp) {
    out("const PassFunc pass_vtable[_PASS_SIZE] = { &pass_error, ");
    for (int i = 0; i < array_size(config->passes); i++) {
        Pass *pass = array_get(config->passes, i);
        out("&pass_%s, ", pass_func_or_id(pass));
    }
    out(" };\n\n");
}

void gen_phase_vtable(Config *config, FILE *fp) {
    out("const PhaseFunc phase_vtable[_PHASE_SIZE] = { &phase_error, ");
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        out("&phase_%s, ", phase->id->lwr);
    }
    out(" };\n\n");
}

void gen_vtables_src(Config *config, FILE *fp) {
    out("#include <stdio.h>\n");
    out("\n");
    out("#include \"ccn/vtables_core.h\"\n");
    out("\n");
    gen_vtables(config, fp);
    gen_trav_data_vtable(config, fp, "Init");
    gen_trav_data_vtable(config, fp, "Free");
    gen_pass_vtable(config, fp);
    gen_phase_vtable(config, fp);
}
