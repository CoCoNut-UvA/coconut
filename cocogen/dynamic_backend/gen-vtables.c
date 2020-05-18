#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/to-string.h"
#include "filegen/driver.h"
#include "filegen/formatting.h"
#include "filegen/genmacros.h"
#include "filegen/reachability.h"
#include "filegen/util.h"
#include "gen-functions.h"
#include "lib/print.h"
#include "lib/smap.h"

static int indent = 0;

void gen_system_vtable(Config *config, FILE *fp, char *version) {
    char *verlwr = strlwr(version);
    out("const TravFunc %s_vtable[_NT_SIZE] = { &trav_error, ", verlwr);
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        char *nodelwr = strlwr(node->id);
        out("&%s_%s, ", verlwr, nodelwr);
        mem_free(nodelwr);
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
    char *travlwr = strlwr(trav->id);
    out("const TravFunc %s_vtable[_NT_SIZE] = { &trav_error, ", travlwr);
    int tindex = get_trav_index(trav->id);
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        char *nodelwr = strlwr(node->id);
        if (is_traversal_node(trav, node)) {
            out("&%s_%s, ", travlwr, nodelwr);
        } else if (is_pass_node(trav, node)) {
            out("&trav_children, ");
        } else {
            out("&trav_return, ");
        }
        mem_free(nodelwr);
    }
    out(" };\n\n");
    mem_free(travlwr);
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
    out("const TravFunc *trav_mat[_TRAV_SIZE] = { error_vtable, ");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("%s_vtable, ", travlwr);
        mem_free(travlwr);
    }
    out("free_vtable, ");
    out("copy_vtable, ");
    out("check_vtable, ");
    out(" };\n\n");
}

void gen_trav_data_vtable(Config *config, FILE *fp, char *version) {
    char *verlwr = strlwr(version);
    out("const %sFunc trav_data_%s_vtable[_TRAV_SIZE] = { ", version, verlwr);
    out("&trav_%s_error, ", verlwr);
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        if (trav->data) {
            out("&trav_%s_%s, ", verlwr, travlwr);
        } else {
            out("&trav_%s_return, ", verlwr);
        }
        mem_free(travlwr);
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
        char *passlwr;
        if (pass->func) {
            passlwr = strlwr(pass->func);
        } else {
            passlwr = strlwr(pass->id);
        }
        out("&pass_%s, ", passlwr);
        mem_free(passlwr);
    }
    out(" };\n\n");
}

void gen_phase_vtable(Config *config, FILE *fp) {
    out("const PhaseFunc phase_vtable[_PHASE_SIZE] = { &phase_error, ");
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        char *phaselwr = strlwr(phase->id);
        out("&phase_%s, ", phaselwr);
        mem_free(phaselwr);
    }
    out(" };\n\n");
}

void gen_vtables_src(Config *config, FILE *fp) {
    out("#include <stdio.h>\n");
    out("\n");
    out("#include \"include/core/vtables_core.h\"\n");
    out("\n");
    gen_vtables(config, fp);
    gen_trav_data_vtable(config, fp, "Init");
    gen_trav_data_vtable(config, fp, "Free");
    gen_pass_vtable(config, fp);
    gen_phase_vtable(config, fp);
}
