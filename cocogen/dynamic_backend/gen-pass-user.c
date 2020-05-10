#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_pass_user_header(Config *config, FILE *fp, Pass *pass) {
    char *passupr;
    if (pass->func) {
        passupr = strupr(pass->func);
    } else {
        passupr = strupr(pass->id);
    }
    char *passlwr;
    if (pass->func) {
        passlwr = strlwr(pass->func);
    } else {
        passlwr = strlwr(pass->id);
    }
    out("#ifndef _CCN_PASS_%s_H_\n", passupr);
    out("#define _CCN_PASS_%s_H_\n\n", passupr);
    out("#include \"inc_core/ast_core.h\"\n");
    out("#include \"inc_core/actions_core.h\"\n");
    out("\n");
    out_field("Node *pass_%s(Node *arg_node)", passlwr);
    out("\n");
    out("#endif /* _CCN_PASS_%s_H_ */\n", passupr);
    free(passlwr);
    free(passupr);
}

void gen_pass_user_src(Config *config, FILE *fp, Pass *pass) {
    char *passlwr;
    if (pass->func) {
        passlwr = strlwr(pass->func);
    } else {
        passlwr = strlwr(pass->id);
    }
    out("#include \"inc_generated/pass_%s.h\"\n", passlwr);
    out("\n");

    out_start_func("Node *pass_%s(Node *arg_node)", passlwr);
    out_comment("User code here");
    out_field("return arg_node");
    out_end_func();

    free(passlwr);
}