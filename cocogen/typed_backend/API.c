#include "API.h"
#include "commandline.h"
#include "filegen/driver.h"
#include "filegen/util.h"
#include "lib/assert.h"

#include "gen-functions.h"

extern void gen_action_array_h(Config *c, FILE *fp);
extern void gen_action_array_c(Config *c, FILE *fp);

extern void generate_enables(Config *c, FILE *fp);

static void generate_headers(Config *ir) {
    set_current_directory_to_be_tracked(global_command_options.header_dir);
    filegen_dir(global_command_options.header_dir);

    filegen_generate("enum.h", generate_enum_definitions);
    filegen_generate("ast.h", generate_ast_definitions);
    filegen_all_nodes("ast-%s.h", generate_ast_node_header);
    filegen_all_nodesets("ast-%s.h", generate_ast_nodeset_header);

    filegen_generate("free-ast.h", generate_free_header);
    filegen_all_nodes("free-%s.h", generate_free_node_header);
    filegen_all_nodesets("free-%s.h", generate_free_nodeset_header);

    filegen_generate("create-ast.h", generate_create_header);
    filegen_all_nodes("create-%s.h", generate_create_node_header);
    filegen_all_nodesets("create-%s.h", generate_create_nodeset_header);

    filegen_generate("copy-ast.h", generate_copy_header);
    filegen_all_nodes("copy-%s.h", generate_copy_node_header);
    filegen_all_nodesets("copy-%s.h", generate_copy_nodeset_header);

    filegen_all_traversals("traversal-%s.h", generate_user_trav_header);
    filegen_all_passes("pass-%s.h", generate_pass_header);

    filegen_generate("gate_functions.h", generate_gate_headers);

    filegen_generate("trav-ast.h", generate_trav_header);
    filegen_generate("trav-core.h", generate_trav_core_header);
    filegen_all_nodes("trav-%s.h", generate_trav_node_header);

    // TODO(Damian): serialization generation.
    filegen_generate("action_handlers.h", gen_action_array_h);
    filegen_generate("ccn_enables.h", generate_enables);
}

static void generate_sources(Config *ir) {
    set_current_directory_to_be_tracked(global_command_options.source_dir);
    filegen_dir(global_command_options.source_dir);

    filegen_all_nodes("free-%s.c", generate_free_node_definitions);
    filegen_all_nodesets("free-%s.c", generate_free_nodeset_definitions);

    filegen_all_nodes("create-%s.c", generate_create_node_definitions);
    filegen_all_nodesets("create-%s.c", generate_create_nodeset_definitions);

    filegen_all_nodes("copy-%s.c", generate_copy_node_definitions);
    filegen_all_nodesets("copy-%s.c", generate_copy_nodeset_definitions);

    filegen_generate("trav-core.c", generate_trav_core_definitions);
    filegen_all_nodes("trav-%s.c", generate_trav_node_definitions);

    filegen_generate("action_handlers.c", gen_action_array_c);
}

void typed_backend(Config *ir) {
    ccn_assert(global_command_options.header_dir != NULL, "No header dir");
    ccn_assert(global_command_options.source_dir != NULL, "No source dir");

    generate_headers(ir);
    generate_sources(ir);
}
