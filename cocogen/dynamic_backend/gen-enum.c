#include <stdio.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "filegen/util.h"
#include "gen-functions.h"

static int indent = 0;

void gen_enum(Config *config, FILE *fp, Enum *arg_enum) {
    out_comment("Enum %s", arg_enum->id->orig);
    out_enum("%s", arg_enum->id->upr);
    out_enum_field("%s_NULL", arg_enum->prefix->lwr);
    for (int i = 0; i < array_size(arg_enum->values); i++) {
        char *id = (char *)array_get(arg_enum->values, i);
        out_enum_field("%s_%s", arg_enum->prefix->lwr, id);
    }
    out_enum_field("_%s_SIZE", arg_enum->prefix->lwr);
    out_enum_end("%s", arg_enum->id->orig);
}

void gen_nodetype_enum(Config *config, FILE *fp) {
    out_comment("Enum " NT_ENUM_NAME);
    out_enum("NODETYPE");
    out_enum_field(NT_ENUM_PREFIX "NULL");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = (Node *)array_get(config->nodes, i);
        out_enum_field(NT_ENUM_PREFIX "%s", node->id->lwr);
    }
    out_enum_field("_" NT_ENUM_PREFIX "SIZE");
    out_enum_end(NT_ENUM_NAME);
}

void gen_nodeset_enum(Config *config, FILE *fp) {
    out_comment("Enum NodeSetType");
    out_enum("NODESETTYPE");
    out_enum_field(NS_ENUM_PREFIX "NULL");
    for (int i = 0; i < array_size(config->nodesets); ++i) {
        Nodeset *nodeset = (Nodeset *)array_get(config->nodesets, i);
        out_enum_field(NS_ENUM_PREFIX "%s", nodeset->id->lwr);
    }
    out_enum_field("_" NS_ENUM_PREFIX "SIZE");
    out_enum_end("NodesetType");
}

void gen_traversal_enum(Config *config, FILE *fp) {
    out_comment("Enum TraversalType");
    out_enum("TRAVERSALS");
    out_enum_field(TRAV_ENUM_PREFIX "NULL");
    for (int i = 0; i < array_size(config->traversals); ++i) {
        Traversal *trav = (Traversal *)array_get(config->traversals, i);
        out_enum_field(TRAV_ENUM_PREFIX "%s", trav->id->lwr);
    }
    out_enum_field(TRAV_ENUM_PREFIX "free");
    out_enum_field(TRAV_ENUM_PREFIX "copy");
    out_enum_field(TRAV_ENUM_PREFIX "check");
    out_enum_field("_" TRAV_ENUM_PREFIX "SIZE");
    out_enum_end("TraversalType");
}

void gen_pass_enum(Config *config, FILE *fp) {
    out_comment("Enum PassType");
    out_enum("PASSES");
    out_enum_field("PASS_NULL");
    for (int i = 0; i < array_size(config->passes); ++i) {
        Pass *pass = (Pass *)array_get(config->passes, i);
        char *passid = NULL;
        if (pass->func) {
            passid = pass->func->lwr;
        } else {
            passid = pass->id->lwr;
        }
        out_enum_field("PASS_%s", passid);
    }
    out_enum_field("_PASS_SIZE");
    out_enum_end("PassType");
}

void gen_phase_enum(Config *config, FILE *fp) {
    out_comment("Enum PhaseType");
    out_enum("PHASES");
    out_enum_field("PHASE_NULL");
    for (int i = 0; i < array_size(config->phases); ++i) {
        Phase *phase = (Phase *)array_get(config->phases, i);
        out_enum_field("PHASE_%s", phase->id->lwr);
    }
    out_enum_field("_PHASE_SIZE");
    out_enum_end("PhaseType");
}

void gen_enum_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_ENUM_H_\n");
    out("#define _CCN_ENUM_H_\n\n");
    gen_nodetype_enum(config, fp);
    gen_nodeset_enum(config, fp);
    gen_traversal_enum(config, fp);
    gen_phase_enum(config, fp);
    gen_pass_enum(config, fp);
    for (int i = 0; i < array_size(config->enums); ++i) {
        Enum *arg_enum = (Enum *)array_get(config->enums, i);
        gen_enum(config, fp, arg_enum);
    }
    out("#endif /* _CCN_ENUM_H_ */\n");
}

void gen_enum_src(Config *config, FILE *fp) {}
