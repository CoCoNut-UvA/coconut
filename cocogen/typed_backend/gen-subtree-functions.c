/** In the DSL the user has the option to specify the
 * root in a phase. This is done by saying root = <node name>.
 * This then takes a sub-tree from the full AST and do
 * actions only on the sub-tree. However, we do not take
 * one sub tree, we take all sub-trees that have, as their
 * root, the specified root. Therefore, we need to execute
 * the action list on a number of sub-trees.
 * In order to achieve this we generate traversals
 * that find the newly specified root, now named the
 * sub-root. The sub-root will be found using a traversal
 * and placed in a special struct in the phase driver.
 * This will then be used in the corresponding phase.
 */


//TODO(damian) PRIORITY: make sure removing a root also removes the files.

#include <string.h>
#include "lib/set.h"
#include "lib/str.h"
#include "lib/set_implementations.h"
#include "ast/ast.h"
#include "ast/create.h"
#include "filegen/util.h"
#include "filegen/driver.h"


static inline void generate_include(char *filename, FILE *fp) {
    out("#include \"%s\"\n", filename);
}

void subtree_generate_call_find_sub_root(char *root, char *to_find, FILE *fp, Phase *phase, int *_indent) {
    int indent = *_indent;
    out_statement("trav_start_%s(temp_root, TRAV__CCN_PhaseDriver_Find%sFrom%s)", root, to_find, root);
    out_statement("root = _ccn_subroot_get_%s()", to_find);
    out_begin_if("root == NULL");
    out_statement("printf(\"Could not find sub-root %s from parent %s\\n\")", to_find, root);
    out_statement("_ccn_end_phase(\"%s\")", phase->id);
    out_statement("return");
    out_end_if();
    out_statement("_ccn_destroy_sub_root()");
    *_indent = indent;
}

/* Generates the function to set the sub-root for a defined nodetype/
 */
void subtree_generate_set_handler(char *root, ccn_set_t *funcs, Config *config) {
    if (ccn_set_contains(funcs, root))
        return;

    char *full_file = ccn_str_cat(config->header_dir, "_sub_root_handlers.h");
    char *source_file = ccn_str_cat(config->source_dir, "_sub_root_handlers.c");

    FILE *fp = get_fp(full_file, "a");
    {
        out("#include <stdio.h>\n");
        out("void _ccn_subroot_set_%s(%s *sub_root);\n", root, root);
        out("void *_ccn_subroot_get_%s();\n", root);
    }
    fclose(fp);
    mem_free(full_file);

    fp = get_fp(source_file, "a");
    {
        generate_include("lib/memory.h", fp);
        out("void _ccn_subroot_set_%s(%s *sub_root) {\n", root, root);
        out("phase_driver_t *phase_driver = _get_phase_driver();\n");
        out("if (phase_driver->curr_sub_root == NULL) {\n");
        out("phase_driver->curr_sub_root = mem_alloc(sizeof(struct ccn_subroot));\n}\n");
        out("phase_driver->curr_sub_root->value = sub_root;\n");
        out("phase_driver->curr_sub_root->nodetype = NT_%s;\n", root);
        out("}\n\n");
        out("void *_ccn_subroot_get_%s() {\n", root);
        out("phase_driver_t *pd = _get_phase_driver();\n");
        out("if (pd->curr_sub_root->nodetype != NT_%s) {\n", root);
        out("return NULL; \n }\n");
        out("return pd->curr_sub_root->value;\n");
        out("}\n\n");
    }
    fclose(fp);
    mem_free(source_file);

    ccn_set_insert(funcs, strdup(root));
}


void subtree_generate_find_traversal_body(char *trav_name, char *target, Config *config) {
    char *file = ccn_str_cat(trav_name, ".c");
    char *full_file = ccn_str_cat(config->source_dir, file);
    add_filename_to_tracked_specify_dir(config->source_dir, file);
    FILE *fp = get_fp(full_file, "w");
    {
        out("#include \"generated/traversal-%s.h\"\n", trav_name);
        generate_include("generated/ast.h", fp);
        generate_include("generated/_sub_root_handlers.h", fp);
        out("#include <stddef.h>\n");
        out("#include <stdio.h>\n");
        out("struct Info {void *empty;};\n");
        out("Info *%s_createinfo(void) { return NULL;}\n", trav_name);
        out("void %s_freeinfo(Info *info) {}\n", trav_name);
        out("void %s_%s(%s *node, Info *info) {\n", trav_name, target, target);
        out("_ccn_subroot_set_%s(node);\n", target);
        out("}\n\n");
    }
    fclose(fp);

    print_file_gen(full_file);
    mem_free(full_file);
}


// TODO cleanup arguments, place in struct or so.
void subtree_generate_find_traversal(Phase *phase, char *root, Node *original_root, Config *config, ccn_set_t *gen, ccn_set_t *funcs) {
    if (original_root == NULL)
	    exit(1); // TODO: place contract here.

    Node *target_root = NULL;
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        if (strcmp(node->id, root) == 0) {
            target_root = node;
            break;
        }
    }

    for (int i = 0; i < array_size(phase->actions); ++i) {
        Action *action = array_get(phase->actions, i);
        if (action->type == ACTION_PHASE) {
            Phase *child = action->action;
            if (child->root != NULL) {
                subtree_generate_find_traversal(child, child->root, target_root, config, gen, funcs);
            }
        }
    }

    char *target = ccn_str_cat(root, original_root->id);
    if (!ccn_set_insert(gen, target)) { // Set is owner of target.
        mem_free(target);
        return;
    }

    ccn_sub_root_pair *sub_root_pair = mem_alloc(sizeof(ccn_sub_root_pair));
    sub_root_pair->from = ccn_str_cpy(original_root->id);
    sub_root_pair->to = ccn_str_cpy(root);
    array_append(config->sub_root_pairs, sub_root_pair);

    // TODO: Fix this using ccn_str_cat_n;
    char *trav_name = ccn_str_cat("_CCN_PhaseDriver_Find", root);
    char *trav_name_sec = ccn_str_cat(trav_name, "From");
    mem_free(trav_name);
    trav_name = ccn_str_cat(trav_name_sec, original_root->id);
    mem_free(trav_name_sec);
    array *literal = create_array();
    array_append(literal, strdup(root));
    SetExpr *expr = create_set_expr(SET_LITERAL, literal);
    Traversal *trav = create_traversal(trav_name, NULL, ccn_str_cpy("CCN"), expr, NULL);
    array_append(config->traversals, trav);
    subtree_generate_set_handler(root, funcs, config);
    subtree_generate_find_traversal_body(trav_name, root, config);
}

void subtree_generate_handlers(const char *header_dir, const char *source_dir) {
    if (is_only_list_files())
        return;

    int indent = 0;

    add_filename_to_tracked_specify_dir(header_dir, "_sub_root_handlers.h");
    add_filename_to_tracked_specify_dir(source_dir, "_sub_root_handlers.c");

    char *header = ccn_str_cat(header_dir, "_sub_root_handlers.h");
    char *source = ccn_str_cat(source_dir, "_sub_root_handlers.c");

    FILE *fp = get_fp(header, "w");
    {
        generate_include("generated/ast.h", fp);
        out_struct("ccn_subroot");
        out_field("NodeType nodetype");
        out_field("void *value");
        out_struct_end();
        fclose(fp);
        //add_filename_to_set(source);
        print_file_gen(header);
    }

    fp = get_fp(source, "w");
    {
        out("#include \"generated/enum.h\"\n");
        generate_include("core/internal_phase_functions.h", fp);
        generate_include("generated/_sub_root_handlers.h", fp);
        print_file_gen(source);
        out("\n");
        fclose(fp);
    }
}

void subtree_generate_traversals(Config *config) {
    subtree_generate_handlers(config->header_dir, config->source_dir);
    array *phases = config->phases;
    ccn_set_t *generated = ccn_set_string_create(10);
    ccn_set_t *generated_funcs = ccn_set_string_create(10);

    for (int i = 0; i < array_size(phases); ++i) {
        Phase *phase = array_get(phases, i);
        if (phase->root == NULL)
            continue;
        subtree_generate_find_traversal(phase, phase->root, config->root_node, config, generated, generated_funcs);
    }
    ccn_set_free(generated);
    ccn_set_free(generated_funcs);
}

void generate_phase_body(Phase *phase, FILE *fp, char *root, bool is_top_root);

static void require_phase_root(Phase *phase, char *root, ccn_set_t *generated, Config *config) {
    if(is_only_list_files())
        return;

    char *target = ccn_str_cat(phase->id, root);
    if (!ccn_set_insert(generated, target)) {
        mem_free(target);
        return;
    }
    char *header = get_full_path_with_dir(config->header_dir, "phase-%s.h", phase->id);
    char *source = get_full_path_with_dir(config->source_dir, "phase-%s.c", phase->id);

    // Hash matches so do not append, will create a redefinition.
    if (!phase->common_info->hash_matches && phase->root_owner) {
        FILE *fp = get_fp(header, "a");
        out("void %s_%s(%s *root);\n", phase->id, root, root);
        fclose(fp);

        fp = get_fp(source, "a");
        out("\n");
        //generate_phase_body(phase, fp, root, false);
        out("\n");
        fclose(fp);
    }
    mem_free(header);
    mem_free(source);
}

/* Mark function allow a typesafe way to place marks.
 * We could do a void*, but that would lose type safety, so generate type dependant functions.
 */
void generate_mark_functions(Config *config, array *phases) {
    ccn_set_t *generated = ccn_set_string_create(10);
    char *header = ccn_str_cat(config->header_dir, "_cycle.h");
    char *source = ccn_str_cat(config->source_dir, "_cycle.c");
    add_filename_to_tracked_specify_dir(config->header_dir, "_cycle.h");
    add_filename_to_tracked_specify_dir(config->source_dir, "_cycle.c");

    int indent = 0;

    FILE *fp = get_fp(header, "w");
    out("#pragma once\n\n");
    for (int i = 0; i < array_size(phases); ++i) {
        Phase *phase = array_get(phases, i);
        if (! phase->root) continue;
        char *root_copy = ccn_str_cpy(phase->root);
        if (ccn_set_insert(generated, root_copy)) {
            out("bool ccn_mark_apply_%s(%s *root);\n", phase->root, phase->root);
            out("bool ccn_mark_remove_%s(%s *root);\n", phase->root, phase->root);
        } else {
            mem_free(root_copy);
        }
    }
    out("\n");
    fclose(fp);

    fp = get_fp(source, "w");
    out("#include \"generated/ast.h\"\n");
    out("#include \"core/internal_phase_functions.h\"\n");
    out("#include <stdio.h>\n\n");
    array *roots = ccn_set_values(generated);
    for (int i = 0; i < array_size(roots); ++i) {
        char *root = array_get(roots, i);
        out_start_func("bool ccn_mark_apply_%s(%s *root)", root, root);
        out_statement("phase_frame_t *top = _top_frame()");
        out_begin_if("top->curr_root != NT_%s", root);
        out_statement("printf(\"Given root does not match with current top.\\n\")");
        out_statement("return false");
        out_end_if();
        out_statement("return _ccn_mark_apply(root)");
        out_end_func();

        out_start_func("bool ccn_mark_remove_%s(%s *root)", root, root);
        out_statement("phase_frame_t *top = _top_frame()");
        out_begin_if("top->curr_root != NT_%s", root);
        out_statement("printf(\"Given root does not match with current top.\\n\")");
        out_statement("return false");
        out_end_if();
        out_statement("return _ccn_mark_remove(root)");
        out_end_func();
    }
    out("\n");
    fclose(fp);
    array_cleanup(roots, NULL);
    ccn_set_free(generated);
}

void subtree_generate_phase_functions(Config *config, array *phases) {
    generate_mark_functions(config, phases);
    ccn_set_t *generated = ccn_set_string_create(10);
    for (int i = 0; i < array_size(phases); ++i) {
        Phase *phase = array_get(phases, i);
        array *values = ccn_set_values(phase->roots);
        for (int j = 0; j < array_size(values); ++j) {
            char *root = array_get(values, j);
            require_phase_root(phase, root, generated, config);
        }
        array_cleanup(values, NULL);

    }
    ccn_set_free(generated);
}
