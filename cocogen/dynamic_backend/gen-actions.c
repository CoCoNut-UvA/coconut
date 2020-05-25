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
    out("#include \"ccn/ast_core.h\"\n");
    out("#include \"ccn/trav_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->traversals); i++) {
        Traversal *trav = array_get(config->traversals, i);
        out_comment("Traversal %s", trav->id->orig);
        if (trav->data) {
            out_field("Trav *%s_init_trav_data(Trav *)", trav->id->lwr);
            out_field("void %s_free_trav_data(Trav *trav)", trav->id->lwr);
        }
        for (int i = 0; i < array_size(trav->nodes); i++) {
            Node *node = array_get(trav->nodes, i);
            out_field("Node *%s_%s(Node *arg_node)", trav->id->lwr,
                      node->id->lwr);
        }
        out("\n");
    }
    out_comment("Traversal Copy");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        out_field("Node *copy_%s(Node *arg_node)", node->id->lwr);
    }
    out("\n");
    out_comment("Traversal Free");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        out_field("Node *free_%s(Node *arg_node)", node->id->lwr);
    }
    out("\n");
    out_comment("Traversal Check");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        out_field("Node *check_%s(Node *arg_node)", node->id->lwr);
    }
    out("\n");
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        out_comment("Phase %s", phase->id->orig);
        out_field("Node *phase_%s(Node *root)", phase->id->lwr);
        out("\n");
    }
    for (int i = 0; i < array_size(config->passes); i++) {
        Pass *pass = array_get(config->passes, i);
        out_comment("Pass %s", pass->id->orig);
        out_field("Node *pass_%s(Node *arg_node)", pass_func_or_id(pass));
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
            out_field("root = pass_start(root, PASS_%s)",
                      pass_func_or_id(action_pass));
            break;
        case ACTION_PHASE:;
            Phase *action_phase = (Phase *)action->action;
            out_field("root = phase_start(root, PHASE_%s)",
                      action_phase->id->lwr);
            break;
        case ACTION_TRAVERSAL:;
            Traversal *action_trav = (Traversal *)action->action;
            out_field("root = trav_start(root, TRAV_%s)", action_trav->id->lwr);
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
    out("#include \"ccn/actions_core.h\"\n");
    out("\n");
    out_start_func("Node *start_root_phase()");
    out_field("Node *root = phase_start(NULL, PHASE_%s)",
              config->start_phase->id->lwr);
    out_field("root = trav_start(root, TRAV_free)");
    out_field("return root");
    out_end_func();
    for (int i = 0; i < array_size(config->phases); i++) {
        Phase *phase = array_get(config->phases, i);
        char *type;
        out_start_func("Node *phase_%s(Node *root)", phase->id->lwr);
        gen_phase(config, fp, phase);
        out_field("root = trav_start(root, TRAV_check)");
        out_field("return root");
        out_end_func();
    }
    out("\n");
}

void gen_pass_user_src(Config *config, FILE *fp, Pass *pass) {
    out("#include \"ccn/actions_core.h\"\n");
    out("\n");
    out_start_func("Node *pass_%s(Node *arg_node)", pass_func_or_id(pass));
    out_comment("User code here");
    out_field("return arg_node");
    out_end_func();
}