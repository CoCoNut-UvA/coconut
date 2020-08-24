

#include "ast/ast.h"
#include "globals.h"
#include "filehandling/gen-driver.h"

void GenNodeTypeEnum(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT_ENUM("ccn_nodetype");
    OUT_ENUM_FIELD("%sNULL", globals.settings->nodetype_enum_prefix);
    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        OUT_ENUM_FIELD("%s%s", globals.settings->nodetype_enum_prefix, node->name->upr);
    }
    OUT_ENUM_FIELD("_%sSIZE", globals.settings->nodetype_enum_prefix);
    OUT_ENUM_END();
}

static
void GenNodesetEnum(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT_ENUM("ccn_nodesettype");
    OUT_ENUM_FIELD("%sNULL", globals.settings->nodeset_enum_prefix);

    struct nodeset *nodeset;
    STAILQ_FOREACH(nodeset, ast->nodesets, next) {
        OUT_ENUM_FIELD("%s%s", globals.settings->nodeset_enum_prefix, nodeset->name->orig);
    }
    OUT_ENUM_FIELD("_%sSIZE", globals.settings->nodeset_enum_prefix);
    OUT_ENUM_END();
}

static
void GenTraversalEnum(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT_ENUM("ccn_traversal_type");
    OUT_ENUM_FIELD("TRAV_NULL");

    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        OUT_ENUM_FIELD("TRAV_%s", trav->name->upr);
    }
    // TODO: we should just inject these traversals in the traverlsal list. This is error prone.
    OUT_ENUM_FIELD("TRAV_free");
    OUT_ENUM_FIELD("TRAV_copy");
    OUT_ENUM_FIELD("TRAV_check");
    OUT_ENUM_FIELD("_TRAV_SIZE");
    OUT_ENUM_END();
}

static
void GenPassEnum(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT_ENUM("ccn_pass_type");
    OUT_ENUM_FIELD("PASS_NULL");

    struct pass *pass = NULL;
    STAILQ_FOREACH(pass, ast->passes, next) {
        OUT_ENUM_FIELD("PASS_%s", pass->name->upr);
    }

    OUT_ENUM_FIELD("_PASS_SIZE");
    OUT_ENUM_END();
}

void GenPhaseEnum(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT_ENUM("ccn_phase_type");
    OUT_ENUM_FIELD("PHASE_NULL");

    struct phase *phase = NULL;
    STAILQ_FOREACH(phase, ast->phases, next) {
        OUT_ENUM_FIELD("PHASE_%s", phase->name->upr);
    }

    OUT_ENUM_FIELD("_PHASE_SIZE");
    OUT_ENUM_END();
}

void GenEnum(struct ast *ast, FILE *fp, struct ccn_enum *arg_enum) {
    int indent = 0;
    OUT_ENUM("%s", arg_enum->name->lwr);
    OUT_ENUM_FIELD("%s_NULL", arg_enum->prefix->upr);

    struct id *id = NULL;
    SLIST_FOREACH(id, arg_enum->values, next) {
        OUT_ENUM_FIELD("%s_%s", arg_enum->prefix->upr, id->lwr);
    }

    OUT_ENUM_FIELD("_%s_SIZE", arg_enum->prefix->upr);
    OUT_ENUM_END();
}


void GenHeaderFile(struct ast *ast, FILE *fp)
{
    int indent = 0;
    OUT("#pragma once\n");
    GenNodeTypeEnum(ast, fp);
    GenNodesetEnum(ast, fp);
    GenPassEnum(ast, fp);
    GenTraversalEnum(ast, fp);
    GenPhaseEnum(ast, fp);

    struct ccn_enum *arg_enum = NULL;
    STAILQ_FOREACH(arg_enum, ast->enums, next) {
        GenEnum(ast, fp, arg_enum);
    }
}

struct ast *GENMdoGen(struct ast *ast)
{
    FILE *fp = FHgetIncludeFile("enum.h");
    GenHeaderFile(ast, fp);

    return ast;
}