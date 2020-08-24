
#include "ast/ast.h"
#include "format/out_macros.h"
#include "filehandling/gen-driver.h"

static
void GenNodeForTraversal(struct ast *ast, FILE *fp, int indent, struct traversal *trav, struct node *node)
{
    char *prefix = trav->name->orig;
    if (trav->prefix != NULL) {
        prefix = trav->prefix->upr;
    }
    OUT_FIELD("struct ccn_node *%s%s(struct ccn_node *node)", prefix, node->name->lwr);
}

static
void GenActionsHeader(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT("#pragma once\n");
    OUT("#include \"ccn/ccn_types.h\"\n");
    OUT("#include \"ccn/dynamic_core.h\"\n");
    OUT("\n");

    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        if (trav->nodes == NULL) {
            struct node *node = NULL;
            STAILQ_FOREACH(node, ast->nodes, next) {
                GenNodeForTraversal(ast, fp, indent, trav, node);
            }
        } else {
            struct node_wrapper *wrapper = NULL;
            SLIST_FOREACH(wrapper, trav->nodes, next) {
                GenNodeForTraversal(ast, fp, indent, trav, wrapper->node);
            }
        }
    }
    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        OUT_FIELD("struct ccn_node *CPY%s(struct ccn_node *arg_node)", node->name->lwr);
        OUT_FIELD("struct ccn_node *DEL%s(struct ccn_node *arg_node)", node->name->lwr);
        OUT_FIELD("struct ccn_node *CHK%s(struct ccn_node *arg_node)", node->name->lwr);
    }

//    for (int i = 0; i < array_size(config->traversals); i++) {
//        Traversal *trav = array_get(config->traversals, i);
//        out_comment("Traversal %s", trav->id->orig);
//        if (trav->data) {
//            out_field("Trav *%s_init_trav_data(Trav *)", trav->id->lwr);
//            out_field("void %s_free_trav_data(Trav *trav)", trav->id->lwr);
//        }
//        for (int i = 0; i < array_size(trav->nodes); i++) {
//            Node *node = array_get(trav->nodes, i);
//            out_field("Node *%s_%s(Node *arg_node)", trav->id->lwr,
//                      node->id->lwr);
//        }
//        out("\n");
//    }
//    out_comment("Traversal Copy");
//    for (int i = 0; i < array_size(config->nodes); ++i) {
//        Node *node = array_get(config->nodes, i);
//        out_field("Node *ccn_copy_%s(Node *arg_node)", node->id->lwr);
//    }
//    out("\n");
//    out_comment("Traversal Free");
//    for (int i = 0; i < array_size(config->nodes); ++i) {
//        Node *node = array_get(config->nodes, i);
//        out_field("Node *ccn_free_%s(Node *arg_node)", node->id->lwr);
//    }
//    out("\n");
//    out_comment("Traversal Check");
//    for (int i = 0; i < array_size(config->nodes); ++i) {
//        Node *node = array_get(config->nodes, i);
//        out_field("Node *ccn_check_%s(Node *arg_node)", node->id->lwr);
//    }
//    out("\n");
//    for (int i = 0; i < array_size(config->phases); i++) {
//        Phase *phase = array_get(config->phases, i);
//        out_comment("Phase %s", phase->id->orig);
//        out_field("Node *phase_%s(Node *root)", phase->id->lwr);
//        out("\n");
//    }

    struct pass *pass = NULL;
    STAILQ_FOREACH(pass, ast->passes, next) {
        char *prefix = "";
        char *func = pass->name->orig;
        if (pass->prefix != NULL) {
            prefix = pass->prefix->upr;
        }
        if (pass->target_func != NULL) {
            func = pass->target_func->orig;
        }
        OUT_FIELD("struct ccn_node *%s%s(struct ccn_node *)", prefix, func);
    }
}


//
//void gen_pass_user_src(Config *config, FILE *fp, Pass *pass) {
//    out("#include \"ccn/actions_core.h\"\n");
//    out("\n");
//    out_start_func("Node *pass_%s(Node *arg_node)", pass_func_or_id(pass));
//    out_comment("User code here");
//    out_field("return arg_node");
//    out_end_func();
//}

struct ast *GAdoGen(struct ast *ast)
{
    FILE *fp = FHgetIncludeFile("actions.h");
    GenActionsHeader(ast, fp);

    return ast;
}