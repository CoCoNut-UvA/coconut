#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_actions_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_ACTIONS_H_\n");
    out("#define _CCN_ACTIONS_H_\n\n");
    out("#include \"include/core/ast_core.h\"\n");
    out("#include \"include/core/trav_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        char *travupr = strupr(trav->id);
        out_comment("Traversal %s", trav->id);
        if (trav->data) {
            out_field("Trav *%s_init_trav_data(Trav *)", travlwr);
            out_field("void %s_free_trav_data(Trav *trav)", travlwr);
        }
        for (int i = 0; i < array_size(trav->nodes); i++) {
            Node *node = array_get(trav->nodes, i);
            char *nodelwr = strlwr(node->id);
            out_field("Node *%s_%s(Node *arg_node)", travlwr, nodelwr);
            mem_free(nodelwr);
        }
        out("\n");
        mem_free(travlwr);
        mem_free(travupr);
    }
    out_comment("Traversal Copy");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out_field("Node *copy_%s(Node *arg_node)", nodelwr);
        mem_free(nodelwr);
    }
    out("\n");
    out_comment("Traversal Free");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out_field("Node *free_%s(Node *arg_node)", nodelwr);
        mem_free(nodelwr);
    }
    out("\n");
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        char *phaselwr = strlwr(phase->id);
        out_comment("Phase %s", phase->id);
        out_field("Node *phase_%s(Node *root)", phaselwr);
        mem_free(phaselwr);
        out("\n");
    }
    for (int i = 0; i < array_size(config->passes); i++) {
        Pass *pass = array_get(config->passes, i);
        out_comment("Pass %s", pass->id);
        char *passlwr;
        if (pass->func) {
            passlwr = strlwr(pass->func);
        } else {
            passlwr = strlwr(pass->id);
        }
        out_field("Node *pass_%s(Node *arg_node)", passlwr);
        mem_free(passlwr);
        out("\n");
    }
    out("#endif /* _CCN_ACTIONS_H_ */\n");
}

void gen_phase(Config *config, FILE *fp, Phase *phase) {
    for (int i = 0; i < array_size(phase->actions); i++) {
        Action *action = array_get(phase->actions, i);
        switch (action->type) {
        case ACTION_PASS:;
            Pass *action_pass = (Pass *)action->action;
            char *passlwr;
            if (action_pass->func) {
                passlwr = strlwr(action_pass->func);
            } else {
                passlwr = strlwr(action_pass->id);
            }
            out_field("root = pass_start(root, PASS_%s)", passlwr);
            mem_free(passlwr);
            break;
        case ACTION_PHASE:;
            Phase *action_phase = (Phase *)action->action;
            char *phaselwr = strlwr(action_phase->id);
            out_field("root = phase_start(root, PHASE_%s)", phaselwr);
            mem_free(phaselwr);
            break;
        case ACTION_TRAVERSAL:;
            Traversal *action_trav = (Traversal *)action->action;
            char *travlwr = strlwr(action_trav->id);
            out_field("root = trav_start(root, TRAV_%s)", travlwr);
            mem_free(travlwr);
            break;
        case ACTION_REFERENCE:
            // TODO: What is this?
            break;
        }
    }
}

void gen_actions_src(Config *config, FILE *fp) {
    out("#include <stddef.h>\n");
    out("\n");
    out("#include \"include/core/actions_core.h\"\n");
    out("\n");
    out_start_func("Node *start_root_phase()");
    char *rootphaselwr = strlwr(config->start_phase->id);
    out_field("Node *root = phase_start(NULL, PHASE_%s)", rootphaselwr);
    mem_free(rootphaselwr);
    out_field("trav_start(root, TRAV_free)");
    out_end_func();
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        char *phaselwr = strlwr(phase->id);
        char *type;
        out_start_func("Node *phase_%s(Node *root)", phaselwr);
        gen_phase(config, fp, phase);
        out_field("return root");
        out_end_func();
        mem_free(phaselwr);
    }
    out("\n");
}

void gen_pass_user_src(Config *config, FILE *fp, Pass *pass) {
    char *passlwr;
    if (pass->func) {
        passlwr = strlwr(pass->func);
    } else {
        passlwr = strlwr(pass->id);
    }
    out("#include \"include/core/actions_core.h\"\n");
    out("\n");
    out_start_func("Node *pass_%s(Node *arg_node)", passlwr);
    out_comment("User code here");
    out_field("return arg_node");
    out_end_func();

    mem_free(passlwr);
}