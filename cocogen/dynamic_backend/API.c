#include "API.h"
#include "commandline.h"
#include "filegen/driver.h"
#include "filegen/reachability.h"
#include "filegen/util.h"
#include "lib/assert.h"

#include "gen-functions.h"

static void generate_headers(Config *ir) {
    set_current_directory_to_be_tracked(global_command_options.header_dir);
    filegen_dir(global_command_options.header_dir);

    filegen_generate("enum.h", &gen_enum_header);
    filegen_generate("ast.h", &gen_ast_header);
    filegen_generate("trav.h", &gen_trav_header);
    filegen_generate("free.h", &gen_free_header);
    filegen_generate("copy.h", &gen_copy_header);
    filegen_generate("actions.h", &gen_actions_header);
    filegen_all_traversals("trav_%s.h", &gen_trav_user_header);
    filegen_all_passes("pass_%s.h", &gen_pass_user_header);
    filegen_generate("CMakeLists.txt", &gen_header_cmakelists);
}

static void generate_sources(Config *ir) {
    set_current_directory_to_be_tracked(global_command_options.source_dir);
    filegen_dir(global_command_options.source_dir);

    filegen_generate("ast.c", &gen_ast_src);
    filegen_generate("vtables.c", &gen_vtables_src);
    filegen_generate("trav.c", &gen_trav_src);
    filegen_generate("free.c", &gen_free_src);
    filegen_generate("copy.c", &gen_copy_src);
    filegen_generate("actions.c", &gen_actions_src);
    filegen_all_traversals("trav_%s.c", &gen_trav_user_src);
    filegen_all_passes("pass_%s.c", &gen_pass_user_src);
    filegen_generate("CMakeLists.txt", &gen_source_cmakelists);
}

void dynamic_backend(Config *ir) {
    ccn_assert(global_command_options.header_dir != NULL, "No header dir");
    ccn_assert(global_command_options.source_dir != NULL, "No source dir");

    compute_reachable_nodes(ir);
    generate_headers(ir);
    generate_sources(ir);
}