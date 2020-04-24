#include "API.h"
#include "filegen/util.h"
#include "filegen/driver.h"
#include "commandline.h"
#include "lib/assert.h"

#include "gen-functions.h"

static void generate_headers(Config *ir)
{
    set_current_directory_to_be_tracked(global_command_options.header_dir);
    filegen_dir(global_command_options.header_dir);

    filegen_generate("enum.h", generate_enum_definitions);
    filegen_generate("ast.h", generate_ast_definitions);
    filegen_all_nodes("ast-%s.h", generate_ast_node_header);
    filegen_all_nodesets("ast-%s.h", generate_ast_nodeset_header);
}

static void generate_sources(Config *ir)
{
    set_current_directory_to_be_tracked(global_command_options.source_dir);
    filegen_dir(global_command_options.source_dir);

    filegen_all_nodes("free-%s.c", generate_free_node_definitions);
    filegen_all_nodesets("free-%s.c", generate_free_nodeset_definitions);

    filegen_all_nodes("create-%s.c", generate_create_node_definitions);
    filegen_all_nodesets("create-%s.c", generate_create_nodeset_definitions);

    filegen_all_nodes("copy-%s.c", generate_copy_node_definitions);
    filegen_all_nodesets("copy-%s.c", generate_copy_nodeset_definitions);

}

void typed_backend(Config *ir)
{
    ccn_assert(global_command_options.header_dir != NULL, "No header dir");
    ccn_assert(global_command_options.source_dir != NULL, "No source dir");

    generate_headers(ir);
    generate_sources(ir);
}