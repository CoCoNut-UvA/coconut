#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "gen-functions.h"

static int indent = 0;

void gen_actions_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_ACTIONS_H_\n");
    out("#define _CCN_ACTIONS_H_\n\n");
    out("#include \"core/ast_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        char *phaselwr = strlwr(phase->id);
        if (phase->cycle) {
            out_field("void cycle_start_%s(Node *root)", phaselwr);
        } else {
            out_field("void phase_start_%s(Node *root)", phaselwr);
        }
        free(phaselwr);
    }
    for (int i = 0; i < array_size(config->passes); i++) {
        Pass *pass = array_get(config->passes, i);
        char *passlwr = strlwr(pass->id);
        out_field("void pass_start_%s()", passlwr);
        free(passlwr);
    }
    out("\n");
    out("#endif /* _CCN_ACTIONS_H_ */\n");
}

void gen_handle_pass(Config *config, FILE *fp, Pass *pass) {
    if (pass->func) {
        out_field("%s()", pass->func);
    }
}

void gen_handle_phase(Config *config, FILE *fp, Phase *phase) {
    for (int i = 0; i < array_size(phase->actions); i++) {
        Action *action = array_get(phase->actions, i);
        switch (action->type) {
        case ACTION_PASS:;
            Pass *action_pass = (Pass *)action->action;
            char *passlwr = strlwr(action_pass->id);
            out_field("pass_start_%s()", passlwr);
            free(passlwr);
            break;
        case ACTION_PHASE:;
            Phase *action_phase = (Phase *)action->action;
            char *phaselwr = strlwr(action_phase->id);
            if (action_phase->cycle) {
                out_field("cycle_start_%s(root)", phaselwr);
            } else {
                out_field("phase_start_%s(root)", phaselwr);
            }
            free(phaselwr);
            break;
        case ACTION_TRAVERSAL:;
            Traversal *action_trav = (Traversal *)action->action;
            char *travlwr = strlwr(action_trav->id);
            out_field("trav_start(root, TRAV_%s)", travlwr);
            free(travlwr);
            break;
        case ACTION_REFERENCE:
            // TODO: What is this?
            break;
        }
    }
}

void gen_actions_src(Config *config, FILE *fp) {
    out("#include \"generated/actions.h\"\n");
    out("#include \"core/trav_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        char *travlwr = strlwr(trav->id);
        out("#include \"generated/trav_%s.h\"\n", travlwr);
        free(travlwr);
    }
    out("\n");
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        char *phaselwr = strlwr(phase->id);
        if (phase->cycle) {
            out_start_func("void cycle_start_%s(Node *root)", phaselwr);
        } else {
            out_start_func("void phase_start_%s(Node *root)", phaselwr);
        }
        gen_handle_phase(config, fp, phase);
        out_end_func();
        free(phaselwr);
    }
    out("\n");
    for (int i = 0; i < array_size(config->passes); i++) {
        Pass *pass = array_get(config->passes, i);
        char *passlwr = strlwr(pass->id);
        out_start_func("void pass_start_%s()", passlwr);
        gen_handle_pass(config, fp, pass);
        out_end_func();
        free(passlwr);
    }
}