
#include <stdio.h>
#include "ast/ast.h"

#include "globals.h"
#include "semantic/semantic.h"
#include "backends/dynamic/dynamic.h"
#include "palm/memory.h"
#include "reachability/reachability.h"
#include "phase_driver/phase_driver.h"

extern char *yy_filename;
extern struct ast *SPparseDSL(FILE *fp);

struct globals globals;

// TODO:
// - check if attribute or child name conflict.

void SetupSettings()
{
    globals.settings = MEMmalloc(sizeof(struct format_settings));
    globals.settings->basic_node_type = "node_st";
    globals.settings->node_constructor_prefix = "ASTnew";
    globals.settings->nodetype_enum_prefix = "NT_";
    globals.settings->access_macro_uppercase = true;
    globals.settings->nodeset_enum_prefix = "NS_";
    globals.gen_hdr_dir = "ccngen/ccngen/";
    globals.gen_src_dir = "ccngen/";
}

int main(int argc, char *argv[])
{
    globals.filename = "/home/damian/University/coconut-private/cocogen/tests/frontend/pass/phase.ccn";
    //globals.filename = "/home/damian/University/coconut-private/cocogen/tests/frontend/fail/naming_collisions3.ccn";
    globals.line_map = HTnew_Int(25);

    SetupSettings();

    FILE *f = fopen(globals.filename, "r");
    struct ast *ast = SPparseDSL(f);
    struct phase *phase = NULL;
    struct pass *pass = NULL;
    struct traversal *trav = NULL;
    struct action *action = NULL;

    STAILQ_FOREACH(phase, ast->phases, next) {
        SLIST_FOREACH(action, phase->actions, next) {
            if (action->action_type == ACTION_REFERENCE) {
                printf("ref: %s\n", action->reference_to_action);
            } else {
                printf("NOT A REF\n");
            }
        }
        printf("phase: %s\n", phase->name->orig);
    }
    STAILQ_FOREACH(trav, ast->traversals, next) {
        printf("trav: %s\n", trav->name->orig);
    }
    STAILQ_FOREACH(pass, ast->passes, next) {
        printf("pass: %s:%s\n", pass->name->orig, pass->info);
    }

    // Semantic phase
    ast = STdoTrav(ast);
    ast = CEdoTrav(ast);
    ast = ESEdoTrav(ast);


    ast = RBdoTrav(ast);

    struct node *node = NULL;
    STAILQ_FOREACH(node, ast->nodes, next) {
        printf("%s\n", node->name->orig);
    }


    // Gen phase
    ast = GASTdoGen(ast);
    ast = GENMdoGen(ast);
    ast = GVdoGen(ast);
    ast = GAdoGen(ast);
    ast = GFdoGen(ast);
    ast = GCdoGen(ast);
    ast = GCHKdoGen(ast);


    ast = PDdoGen(ast);

    HTmap(globals.line_map, MEMfree);
    HTdelete(globals.line_map);
}
