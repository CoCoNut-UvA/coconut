#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/util.h"
#include "lib/set_implementations.h"

/* Gen the action handling and the creation of the action array .h file, */
void gen_action_array_h(Config *c, FILE *fp) {
    int indent = 0;
    size_t size = 0;

    out("#pragma once\n\n");

    out("#include \"generated/enum.h\"\n");
    out("#include <stddef.h>\n");
    out_statement("void ccn_init_action_array()");
    out_statement("void dispatch_traversals(NodeType type, void *node, "
                  "TraversalType trav_type)");
    out_statement("TraversalType get_sub_root_find_traversals(NodeType from, "
                  "NodeType to)");
    out_statement("size_t get_offset_next(NodeType type)");

    out("enum ACTION_IDS {\n");
    indent++;
    array *traversals = c->traversals;
    for (int i = 0; i < array_size(traversals); ++i) {
        Traversal *trav = array_get(traversals, i);
        out_enum_field("ACTION_ID_%s", trav->id);
        size++;
    }

    array *passes = c->passes;
    for (int i = 0; i < array_size(passes); ++i) {
        Pass *pass = array_get(passes, i);
        out_enum_field("ACTION_ID_%s", pass->id);
        size++;
    }

    array *phases = c->phases;
    for (int i = 0; i < array_size(phases); ++i) {
        Phase *phase = array_get(phases, i);
        if (phase->original_ref == NULL) {
            out_enum_field("ACTION_ID_%s", phase->id);
            size++;
        }
    }

    out("    ACTION_ID_NULL\n};\n");
    indent--;
    out("#ifndef CCN_NUM_OF_ACTIONS\n");
    out("#define CCN_NUM_OF_ACTIONS %lu\n", size);
    out("#endif\n");

    for (int i = 0; i < array_size(c->passes); ++i) {
        Pass *pass = array_get(c->passes, i);
        out_statement("void *dispatch_pass_%s(void *, NodeType)", pass->id);
    }

    out_statement("void ccn_destroy_action_array()");

    out("#ifndef CCN_ROOT_TYPE\n");
    out("#define CCN_ROOT_TYPE NT_%s\n", c->root_node->id);
    out("#endif\n");

    out("#ifndef CCN_ROOT_ACTION\n");
    out("#define CCN_ROOT_ACTION ACTION_ID_%s\n", c->start_phase->id);
    out("#endif\n\n");
}

/* Gen the action handling and the creation of the action array .c file, */
void gen_pass_dispatchers(Config *config, Pass *pass, int indent, FILE *fp) {
    out_start_func("void *dispatch_pass_%s(void *node, NodeType root)",
                   pass->id);
    {
        out_begin_switch("root");
        out_begin_case("NT_%s", config->root_node->id);
        if (pass->func) {
            out_statement("return %s(node)", pass->func);
        } else {
            out_statement("return pass_%s_entry(node)", pass->id);
        }
        out_end_case();

        array *values = ccn_set_values(pass->roots);
        for (int i = 0; i < array_size(values); ++i) {
            char *root = array_get(values, i);
            out_begin_case("NT_%s", root);

            if (pass->func) {
                out_statement("return %s_%s(node)", pass->func, root);
            } else {
                out_statement("return pass_%s_entry_%s(node)", pass->id, root);
            }

            out_end_case();
        }
        array_cleanup(values, NULL);

        out_begin_default_case();
        out_statement("return node");
        out_end_case();
        out_end_switch()
    }
    out_end_func();
}

// void gen_action_array_c(Config *c, FILE *fp) {
//     int indent = 0;

//     out("#include \"generated/enum.h\"\n");
//     out("#include \"generated/action_handlers.h\"\n");
//     out("#include \"core/action_handling.h\"\n");
//     out("#include \"generated/gate_functions.h\"\n");
//     out("#include \"generated/trav-ast.h\"\n");
//     out("#include \"core/internal_phase_functions.h\"\n");
//     out("#include \"lib/memory.h\"\n");
//     out("#include \"lib/str.h\"\n");
//     out("#include <stdbool.h>\n");

//     out_statement("ccn_action_t *get_ccn_action_from_id(enum ACTION_IDS)");

//     for (int i = 0; i < array_size(c->passes); ++i) {
//         Pass *pass = array_get(c->passes, i);
//         out("#include \"generated/pass-%s.h\"\n", pass->id);
//     }

//     // Here we do + 1 to allow for an NULL action in the array, which denotes
//     the end. out_statement("static ccn_action_t
//     action_array[CCN_NUM_OF_ACTIONS + 1] = {{.type = action_NULL}}");

//     for (int i = 0; i < array_size(c->phases); ++i) {
//         Phase *phase = array_get(c->phases, i);
//         if (phase->original_ref != NULL) {
//             continue;
//         }

//         size_t actions = array_size(phase->actions) + 1;
//         out("enum ACTION_IDS %s_ids_table[%lu] = {\n", phase->id, actions);
//         for (int j = 0; j < array_size(phase->actions); ++j) {
//             Action *action = array_get(phase->actions, j);
//             out("ACTION_ID_%s, ", action->id);
//             if ((j + 1)% 3 == 0) {
//                 out("\n");
//             }
//         }
//         out("ACTION_ID_NULL\n");
//         out("};\n");
//     }

//     out_start_func("ccn_action_t *get_ccn_action_from_id(enum ACTION_IDS
//     id)");
//     {
//         out_statement("return &action_array[id]");
//     }
//     out_end_func();

//     out_start_func("ccn_action_t *make_trav_action(ccn_action_t *action,
//     TraversalType trav_type, enum ACTION_IDS id, char *name)");
//     {
//         out_statement("ccn_traversal_t *trav =
//         mem_alloc(sizeof(ccn_traversal_t))"); out_statement("trav->trav_type
//         = trav_type"); out_statement("action->type = action_traversal");
//         out_statement("action->traversal = trav");
//         out_statement("action->action_id = id");
//         out_statement("action->name = ccn_str_cpy(name)");
//         out_statement("return action");
//     }
//     out_end_func();

//     out_start_func("ccn_action_t *make_pass_action(ccn_action_t* action, void
//     *(*func)(void*, NodeType), enum ACTION_IDS id, char *name)");
//     {
//         out_statement("ccn_pass_t *pass = mem_alloc(sizeof(ccn_pass_t))");
//         out_statement("pass->func = func");
//         out_statement("action->type = action_pass");
//         out_statement("action->action_id = id");
//         out_statement("action->pass = pass");
//         out_statement("action->name = ccn_str_cpy(name)");
//         out_statement("return action");
//     }
//     out_end_func();

//     out_start_func("ccn_action_t *make_phase_action(ccn_action_t* action,
//     enum ACTION_IDS id, enum ACTION_IDS *id_table, bool (*gate)(void), char
//     *name, bool is_cycle)");
//     {
//         out_statement("ccn_phase_t *phase = mem_alloc(sizeof(ccn_phase_t))");
//         out_statement("phase->gate_func = gate");
//         out_statement("phase->action_types = id_table");
//         out_statement("phase->is_cycle = is_cycle");
//         out_statement("phase->action_id = id");
//         out_statement("phase->root_type = NT_%s", c->root_node->id);
//         out_statement("action->type = action_phase");
//         out_statement("action->action_id = id");
//         out_statement("action->phase = phase");
//         out_statement("action->name = ccn_str_cpy(name)");
//         out_statement("return action");
//     }
//     out_end_func();

//     out_start_func("void ccn_init_action_array()");
//     {
//         for (int i = 0; i < array_size(c->traversals); ++i) {
//             Traversal *trav = array_get(c->traversals, i);
//             out_statement("make_trav_action(&action_array[ACTION_ID_%s],
//             TRAV_%s, ACTION_ID_%s, \"%s\")",
//                 trav->id, trav->id, trav->id, trav->id);
//         }

//         for (int i = 0; i < array_size(c->passes); ++i) {
//             Pass *pass = array_get(c->passes, i);
//             out_statement("make_pass_action(&action_array[ACTION_ID_%s],
//             &dispatch_pass_%s, ACTION_ID_%s, \"%s\")",
//                 pass->id, pass->id, pass->id, pass->id);
//         }

//         for (int i = 0; i < array_size(c->phases); ++i) {
//             Phase *phase = array_get(c->phases, i);
//             if (phase->original_ref != NULL) {
//                 continue;
//             }
//             char *cycle = phase->cycle ? "true" : "false";
//             if (phase->gate_func != NULL) {
//                 out_statement("make_phase_action(&action_array[ACTION_ID_%s],
//                 ACTION_ID_%s, %s_ids_table, %s, \"%s\", %s)",
//                     phase->id, phase->id, phase->id, phase->gate_func,
//                     phase->id, cycle);
//             } else {
//                 out_statement("make_phase_action(&action_array[ACTION_ID_%s],
//                 ACTION_ID_%s, %s_ids_table, NULL, \"%s\", %s)",
//                     phase->id, phase->id, phase->id, phase->id, cycle);
//             }
//             if (phase->root != NULL) {
//                 out_statement("action_array[ACTION_ID_%s].phase->root_type =
//                 NT_%s", phase->id, phase->root);
//             }
//         }
//         out_statement("action_array[ACTION_ID_NULL].type = action_NULL");

//     }
//     out_end_func();

//     // TODO: can probably be in a lookup table instead.
//     out_start_func("void dispatch_traversals(NodeType type, void *node,
//     TraversalType trav_type)");
//     {
//         out_begin_switch("type");
//         for (int i = 0; i < array_size(c->nodes); ++i) {
//             Node *node = array_get(c->nodes, i);
//             out_begin_case("NT_%s", node->id);
//             out_statement("trav_start_%s(node, trav_type)", node->id);
//             out_statement("break");
//             out_end_case();

//         }

//         out_begin_default_case();
//         out_statement("break");
//         out_end_case();
//         out_end_switch();
//     }
//     out_end_func();

//     for (int i = 0; i < array_size(c->passes); ++i) {
//         Pass *pass = array_get(c->passes, i);
//         gen_pass_dispatchers(c, pass, indent, fp);
//     }

//     out_start_func("TraversalType get_sub_root_find_traversals(NodeType from,
//     NodeType to)");
//     {
//         for (int i = 0; i < array_size(c->sub_root_pairs); ++i) {
//             ccn_sub_root_pair *pair = array_get(c->sub_root_pairs, i);
//             out_begin_if("NT_%s == from && NT_%s == to", pair->from,
//             pair->to); out_statement("return
//             TRAV__CCN_PhaseDriver_Find%sFrom%s", pair->to, pair->from);
//             out_end_if();
//         }
//     }
//     out_statement("return TRAV_NULL");
//     out_end_func();

//     out_start_func("size_t get_offset_next(NodeType type)");
//     ccn_set_t *to_values_already_processed =
//     ccn_set_string_create(array_size(c->sub_root_pairs)); for (int i = 0; i <
//     array_size(c->sub_root_pairs); ++i) {
//         ccn_sub_root_pair *pair = array_get(c->sub_root_pairs, i);
//         if (ccn_set_insert(to_values_already_processed, pair->to)) {
//             out_begin_if("NT_%s == type", pair->to);  // TODO: Check for
//             double entries here in the pair->to values. out_statement("return
//             offsetof(%s, next)", pair->to); out_end_if();
//         }
//     }
//     ccn_set_free_with_func(to_values_already_processed, NULL);
//     out_statement("return 0"); // TODO: Signal error here.
//     out_end_func();

//     out_start_func("void ccn_destroy_action(ccn_action_t *action)");
//     out_statement("mem_free(action->name)");
//     out_begin_if("action->type == action_traversal");
//     out_statement("mem_free(action->traversal)");
//     out_end_if();
//     out_begin_if("action->type == action_pass");
//     out_statement("mem_free(action->pass)");
//     out_end_if();
//     out_begin_if("action->type == action_phase");
//     out_statement("mem_free(action->phase)");
//     out_end_if();
//     out_end_func();

//     out_start_func("void ccn_destroy_action_array()");
//     out_statement("if (action_array[0].type == action_NULL) return");
//     out_statement("size_t index = 0");
//     out_statement("ccn_action_t *action = &action_array[index++]");
//     out_begin_while("action->type != action_NULL");
//     out_statement("ccn_destroy_action(action)");
//     out_statement("action = &action_array[index++]");
//     out_end_while();
//     out_statement("action_array[0].type = action_NULL");
//     out_end_func();
// }

void gen_action_array_c(Config *c, FILE *fp) {
    out("#include \"generated/enum.h\"\n");
    out("#include \"generated/action_handlers.h\"\n");
    out("#include \"core/action_handling.h\"\n");
    out("#include \"generated/gate_functions.h\"\n");
    out("#include \"generated/trav-ast.h\"\n");
    out("#include \"core/internal_phase_functions.h\"\n");
    out("#include \"lib/memory.h\"\n");
    out("#include \"lib/str.h\"\n");
    out("#include <stdbool.h>\n");

    for (int i = 0; i < array_size(c->phases); ++i) {
        Phase *phase = array_get(c->phases, i);
        if (phase->original_ref != NULL) {
            continue;
        }

        size_t actions = array_size(phase->actions) + 1;
        out("enum ACTION_IDS %s_ids_table[%lu] = {\n", phase->id, actions);
        for (int j = 0; j < array_size(phase->actions); ++j) {
            Action *action = array_get(phase->actions, j);
            out("ACTION_ID_%s, ", action->id);
            if ((j + 1) % 3 == 0) {
                out("\n");
            }
        }
        out("ACTION_ID_NULL\n");
        out("};\n");
    }

    /// Order is important and should match the order of the action id, so:
    /// traversals, phases and passes.
    out("static ccn_action_t action_array[] = {\n");
    for (size_t i = 0; i < array_size(c->traversals); i++) {
        Traversal *t = array_get(c->traversals, i);
        out("{action_traversal, ACTION_ID_%s, \"%s\", .traversal = "
            "{TRAV_%s}},\n",
            t->id, t->id, t->id);
    }

    for (size_t i = 0; i < array_size(c->phases); i++) {
        Phase *p = array_get(c->phases, i);
        char *root = NULL;
        if (p->root != NULL) {
            root = p->root;
        } else {
            root = c->root_node->id;
        }

        out("{action_phase, ACTION_ID_%s, \"%s\", .phase = {NULL, NT_%s, "
            "%s_ids_table, false, ACTION_ID_%s}},\n",
            p->id, p->id, root, p->id, p->id);
    }

    for (size_t i = 0; i < array_size(c->passes); i++) {
        Pass *p = array_get(c->passes, i);
        out("{action_pass, ACTION_ID_%s, \"%s\", .pass = {}}\n", p->id, p->id);
    }

    out("};\n\n");
}