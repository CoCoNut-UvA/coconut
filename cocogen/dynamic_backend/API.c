#include "API.h"
#include "commandline.h"
#include "filegen/driver.h"
#include "filegen/reachability.h"
#include "filegen/util.h"
#include "gen-functions.h"
#include "lib/assert.h"
#include "lib/print.h"

extern void gen_action_array_h(Config *c, FILE *fp);
extern void gen_action_array_c(Config *c, FILE *fp);

extern void generate_enables(Config *c, FILE *fp);

static void generate_headers(Config *ir) {
    set_current_directory_to_be_tracked(global_command_options.header_dir);
    filegen_dir(global_command_options.header_dir);

    filegen_generate("enum.h", &gen_enum_header);
    filegen_generate("ast.h", &gen_ast_header);
    filegen_generate("trav.h", &gen_trav_data_header);
    filegen_generate("actions.h", &gen_actions_header);
    filegen_generate("action_handlers.h", &gen_action_array_h);
    filegen_generate("ccn_enables.h", &generate_enables);
    filegen_generate("CMakeLists.txt", &gen_header_cmakelists);
}

static void generate_sources(Config *ir) {
    set_current_directory_to_be_tracked(global_command_options.source_dir);
    filegen_dir(global_command_options.source_dir);

    filegen_generate("ast.c", &gen_ast_src);
    filegen_generate("trav.c", &gen_trav_data_src);
    filegen_generate("vtables.c", &gen_vtables_src);
    filegen_generate("trav_check.c", &gen_check_src);
    filegen_generate("trav_free.c", &gen_free_src);
    filegen_generate("trav_copy.c", &gen_copy_src);
    filegen_generate("action_handlers.c", gen_action_array_c);
    if (global_command_options.gen_user_files) {
        /**
         * Generate user traversal and pass files.
         * WARNING, THIS WILL OVERWRITE CURRENT FILES
         **/
        print_warning_no_loc(
            "Are you sure you want to overwrite user files? [y/n]");
        char input;
        scanf(" %c", &input);
        if (input == 'y') {
            filegen_all_traversals("trav_%s.c", &gen_trav_user_src);
            filegen_all_passes("pass_%s.c", &gen_pass_user_src);
        }
    }
    filegen_generate("CMakeLists.txt", &gen_source_cmakelists);
}

void dynamic_backend(Config *ir) {
    ccn_assert(global_command_options.header_dir != NULL, "No header dir");
    ccn_assert(global_command_options.source_dir != NULL, "No source dir");

    create_indices(ir);
    compute_reachable_nodes(ir);
    compute_traversal_nodes(ir);
    generate_headers(ir);
    generate_sources(ir);
}