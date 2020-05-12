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
    out("\n");
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        char *phaselwr = strlwr(phase->id);
        char *type;
        if (phase->cycle) {
            type = "cycle";
        } else {
            type = "phase";
        }
        out_field("void %s_start_%s(Node *root)", type, phaselwr);
        mem_free(phaselwr);
    }
    for (int i = 0; i < array_size(config->passes); i++) {
        Pass *pass = array_get(config->passes, i);
        char *passlwr;
        if (pass->func) {
            passlwr = strlwr(pass->func);
        } else {
            passlwr = strlwr(pass->id);
        }
        out_field("void pass_start_%s(Node *root, PassType type)", passlwr);
        mem_free(passlwr);
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
            char *passlwr;
            if (action_pass->func) {
                passlwr = strlwr(action_pass->func);
            } else {
                passlwr = strlwr(action_pass->id);
            }
            out_field("pass_start(root, PASS_%s)", passlwr);
            mem_free(passlwr);
            break;
        case ACTION_PHASE:;
            Phase *action_phase = (Phase *)action->action;
            char *phaselwr = strlwr(action_phase->id);
            char *type;
            if (action_phase->cycle) {
                type = "cycle";
            } else {
                type = "phase";
            }
            out_field("%s_start_%s(root)", type, phaselwr);
            mem_free(phaselwr);
            break;
        case ACTION_TRAVERSAL:;
            Traversal *action_trav = (Traversal *)action->action;
            char *travlwr = strlwr(action_trav->id);
            out_field("trav_start(root, TRAV_%s)", travlwr);
            mem_free(travlwr);
            break;
        case ACTION_REFERENCE:
            // TODO: What is this?
            break;
        }
    }
}

void gen_actions_src(Config *config, FILE *fp) {
    out("#include \"include/generated/actions.h\"\n");
    out("#include \"include/core/trav_core.h\"\n");
    out("#include \"include/core/actions_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        char *phaselwr = strlwr(phase->id);
        char *type;
        if (phase->cycle) {
            type = "cycle";
        } else {
            type = "phase";
        }
        out_start_func("void %s_start_%s(Node *root)", type, phaselwr);
        gen_handle_phase(config, fp, phase);
        out_end_func();
        mem_free(phaselwr);
    }
    out("\n");
}