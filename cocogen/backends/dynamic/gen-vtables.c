
#include "ast/ast.h"
#include "filehandling/gen-driver.h"
#include "format/out_macros.h"
#include "palm/str.h"
#include "reachability/reachability.h"
#include <palm/memory.h>

/* This does it for every 'system' trav. So, copy and such
 */
void GenSystemVtable(struct ast *ast, FILE *fp, const char *table_name, const char *prefix) {
    int indent = 0;
    OUT("const ccn_trav_ft %s_vtable[_NT_SIZE] = { &TRAVerror, ", table_name);
    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        OUT("&%s%s, ", prefix, node->name->lwr);
    }
    OUT(" };\n\n");
}
//
static
void GenErrorVtable(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT("const ccn_trav_ft ccn_error_vtable[_NT_SIZE] = { &TRAVerror, ");
    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        OUT("&TRAVerror, ");
    }
    OUT(" };\n\n");
}
//

void GenNodeForTrav(FILE *fp, int indent, struct traversal *trav, struct node *node)
{
    char *prefix = trav->name->orig;
    if (trav->prefix != NULL) {
        prefix = trav->prefix->upr;
    }
    if (RBisTraversalNode(trav, node)) {
        OUT("&%s%s, ", prefix, node->name->lwr);
    } else if (RBshouldPassNode(trav, node)) {
        OUT("&trav_children");
    } else {
        OUT("&trav_return");
    }
}

void GenTravVtable(struct ast *ast, FILE *fp, struct traversal *trav) {
    int indent = 0;
    OUT("const ccn_trav_ft %s_vtable[_NT_SIZE] = { &TRAVerror, ", trav->name->lwr);

    if (trav->nodes == NULL) {
        struct node *node = NULL;
        STAILQ_FOREACH(node, ast->nodes, next) {
            GenNodeForTrav(fp, indent, trav, node);
        }
    } else {
        struct node_wrapper *wrapper = NULL;
        SLIST_FOREACH(wrapper, trav->nodes, next) {
            GenNodeForTrav(fp, indent, trav, wrapper->node);
        }
    }

    OUT(" };\n\n");
}

static
void GenVtables(struct ast *ast, FILE *fp) {
    GenErrorVtable(ast, fp);

    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        GenTravVtable(ast, fp, trav);
    }
    GenSystemVtable(ast, fp, "ccn_free", "DEL");
    GenSystemVtable(ast, fp, "ccn_copy", "CPY");
    GenSystemVtable(ast, fp, "ccn_check", "CHK");

    int indent = 0;
    OUT("const ccn_trav_ft *ccn_trav_vtable[_TRAV_SIZE] = { ccn_error_vtable, ");
    trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        OUT("%s_vtable, ", trav->name->lwr);
    }
    OUT("ccn_free_vtable, ");
    OUT("ccn_copy_vtable, ");
    OUT("ccn_check_vtable, ");

    OUT(" };\n\n");
}

//void gen_trav_data_vtable(Config *config, FILE *fp, char *version) {
//    char *verlwr = ccn_str_lwr(version);
//    out("const %sFunc trav_data_%s_vtable[_TRAV_SIZE] = { ", version, verlwr);
//    out("&trav_%s_error, ", verlwr);
//    for (int i = 0; i < array_size(config->traversals); i++) {
//        Traversal *trav = array_get(config->traversals, i);
//        if (trav->data) {
//            out("&trav_%s_%s, ", verlwr, trav->id->lwr);
//        } else {
//            out("&trav_%s_return, ", verlwr);
//        }
//    }
//    out("&trav_%s_return, ", verlwr); // Free
//    out("&trav_%s_return, ", verlwr); // Copy
//    out("&trav_%s_return, ", verlwr); // Check
//    out(" };\n\n");
//    mem_free(verlwr);
//}

char *PassFuncOrId(struct pass *pass)
{
    if (pass->target_func != NULL) {
        return pass->target_func->orig;
    }
    return pass->name->orig;
}

void GenPassVtable(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT("const ccn_pass_ft ccn_pass_vtable[_PASS_SIZE] = { &PASSerror, ");

    struct pass *pass = NULL;
    STAILQ_FOREACH(pass, ast->passes, next) {
        char *prefix = "";
        if (pass->prefix != NULL) {
            prefix = pass->prefix->upr;
        }
        OUT("&%s%s, ", prefix, PassFuncOrId(pass));
    }
    OUT(" };\n\n");
}

//void gen_vtables_src(Config *config, FILE *fp) {
//    out("#include <stdio.h>\n");
//    out("\n");
//    out("#include \"ccn/vtables_core.h\"\n");
//    out("\n");
//    gen_vtables(config, fp);
//    gen_trav_data_vtable(config, fp, "Init");
//    gen_trav_data_vtable(config, fp, "Free");
//    gen_pass_vtable(config, fp);
//}

struct ast *GVdoGen(struct ast *ast)
{
    FILE *fp = FHgetSourceFile("vtables.c");
    int indent = 0;
    OUT("#include \"ccngen/actions.h\"\n");
    OUT("#include \"ccn/ccn_types.h\"\n");

    GenVtables(ast, fp);
    GenPassVtable(ast, fp);

    return ast;
}
