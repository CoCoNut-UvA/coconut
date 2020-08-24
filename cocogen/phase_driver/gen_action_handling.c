

#include "ast/ast.h"
#include "format/out_macros.h"
#include "palm/dbug.h"
#include "filehandling/gen-driver.h"

static char *enum_action_pref = "CCNAC_ID";
static char *enum_action_name = "ccn_action_id";


/* Gen the action handling and the creation of the action array .h file, */
static
void GenActionArrayHeader(struct ast *ast, FILE *fp) {
    int indent = 0;
    size_t size = 0;

    OUT("#pragma once\n\n");

    OUT("#include \"ccngen/enum.h\"\n");
    OUT("#include <stddef.h>\n");
//    OUT_STATEMENT("enum ccn_traversal get_sub_root_find_traversals(NodeType from, "
//                  "NodeType to)");
//    OUT_STATEMENT("size_t get_offset_next(NodeType type)");

    OUT_ENUM("%s", enum_action_name);
    /// Order is important and should be passes,  traversals and phases.
    {
        struct pass *pass = NULL;
        STAILQ_FOREACH(pass, ast->passes, next) {
            OUT_ENUM_FIELD("%s_%s", enum_action_pref, pass->name->upr);
            size++;
        }

        struct traversal *trav = NULL;
        STAILQ_FOREACH(trav, ast->traversals, next) {
            OUT_ENUM_FIELD("%s_%s", enum_action_pref, trav->name->upr);
            size++;
        }

        OUT_ENUM_FIELD("%s_%s", enum_action_pref, "free");
        size++;

        struct phase *phase = NULL;
        STAILQ_FOREACH(phase, ast->phases, next) {
            OUT_ENUM_FIELD("%s_%s", enum_action_pref, phase->name->upr);
        }

        OUT_ENUM_FIELD("%s_%s", enum_action_pref, "cleanup");
        size++;
    }

    OUT_ENUM_FIELD("%s_NULL", enum_action_pref);
    OUT_ENUM_END();

    OUT("#ifndef CCN_NUM_OF_ACTIONS\n");
    OUT("#define CCN_NUM_OF_ACTIONS %lu\n", size);
    OUT("#endif\n");

    OUT("#ifndef CCN_ROOT_TYPE\n");
    OUT("#define CCN_ROOT_TYPE NT_%s\n", ast->root_node->name->upr);
    OUT("#endif\n");

    OUT("#ifndef CCN_ROOT_ACTION\n");
    OUT("#define CCN_ROOT_ACTION %s_%s\n", enum_action_pref, ast->start_phase->name->upr);
    OUT("#endif\n\n");
}



void GenActionArraySource(struct ast *ast, FILE *fp) {
    int indent = 0;
    OUT("#include \"ccn/action_types.h\"\n");
    OUT("#include \"ccngen/enum.h\"\n");
    OUT("#include \"ccngen/action_handling.h\"\n");
    // out("#include \"ccngen/gate_functions.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    OUT("#include \"ccn/phase_driver.h\"\n");
    OUT("#include \"palm/memory.h\"\n");
    OUT("#include \"palm/str.h\"\n");
    OUT("#include <stdbool.h>\n");

    struct phase *phase = NULL;
    STAILQ_FOREACH(phase, ast->phases, next) {
        bool is_root = ast->start_phase == phase;
        OUT("enum %s %s_ids_table[] = {\n", enum_action_name, phase->name->lwr);
        struct action *action = NULL;
        SLIST_FOREACH(action, phase->actions, next) {
            char *action_name = NULL;
            if (action->action_type == ACTION_PASS) {
                action_name = action->pass->name->upr;
            } else if (action->action_type == ACTION_TRAVERSAL) {
                action_name = action->traversal->name->upr;
            } else if (action->action_type == ACTION_PHASE) {
                action_name = action->phase->name->upr;
            }
            DBUG_ASSERT(action_name != NULL, "Could not get action name.");

            OUT("%s_%s, ", enum_action_pref, action_name);
        }
        // TODO: add cleanup for the root phase.
        OUT("%s_NULL", enum_action_pref);
        OUT_ENUM_END();
    }

    OUT("enum %s cleanup_ids_table[2] = { %s_free, %s_NULL };\n", enum_action_name, enum_action_pref, enum_action_pref);

    OUT("static struct ccn_action ccn_action_array[] = {\n");

    /// Order is important and should match the order of the action id, so:
    /// passes, traversals, phases.
    struct pass *pass = NULL;
    STAILQ_FOREACH(pass, ast->passes, next) {
        OUT("{CCN_ACTION_PASS, %s_%s, \"%s\", .pass = {PASS_%s,},},\n",
            enum_action_pref, pass->name->upr, pass->name->orig, pass->name->upr);
    }

    struct traversal *trav = NULL;
    STAILQ_FOREACH(trav, ast->traversals, next) {
        OUT("{CCN_ACTION_TRAVERSAL, %s_%s, \"%s\", .traversal = "
                "{TRAV_%s,},},\n", enum_action_pref, trav->name->upr, trav->name->orig, trav->name->upr);
    }

    OUT("{CCN_ACTION_TRAVERSAL, %s_free, \"Free\", .traversal = "
        "{TRAV_free,},},\n", enum_action_pref);

    phase = NULL;
    STAILQ_FOREACH(phase, ast->phases, next) {
        OUT("{CCN_ACTION_PHASE, %s_%s, \"%s\", .phase = {NULL, NT_%s, "
            "%s_ids_table, false, %s_%s,},},\n", enum_action_pref, phase->name->upr, phase->name->orig, ast->root_node->name->upr,
                phase->name->lwr, enum_action_pref, phase->name->upr);
    }

    OUT("{CCN_ACTION_PHASE, %s_cleanup, \"Cleanup\", .phase = {NULL, NT_%s, "
        "cleanup_ids_table, false, %s_cleanup,},},\n",
        enum_action_pref, ast->root_node->name->upr, enum_action_pref);
    OUT("};\n\n");

    OUT_START_FUNC(
        "struct ccn_action *CCNgetActionFromID(enum %s action_id)", enum_action_name);
    OUT_STATEMENT("return &ccn_action_array[action_id]");
    OUT_END_FUNC();
}

struct ast *PDdoGen(struct ast *ast)
{
    FILE *fp = FHgetIncludeFile("action_handling.h");
    GenActionArrayHeader(ast, fp);
    fp = FHgetSourceFile("action_handling.c");
    GenActionArraySource(ast, fp);

    return ast;
}