cmake_minimum_required(VERSION 3.13)

file(GLOB COCONUT_FILES "${CMAKE_CURRENT_LIST_DIR}/ccngen/*.c" "${PROJECT_SOURCE_DIR}/copra/src/*.c")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(DslParser frontend/scanAndParse/dsl-parser.y ${CMAKE_CURRENT_BINARY_DIR}/dsl-parser.c
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/dsl-parser.h)
flex_target(DslTokens frontend/scanAndParse/dsl-lexer.l ${CMAKE_CURRENT_BINARY_DIR}/dsl-tokens.c
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/dsl-lexer.h
        )
ADD_FLEX_BISON_DEPENDENCY(DslTokens DslParser)

add_executable(cocogen main.c commandline.c frontend/scanAndParse/scan_parse.c
        ${COCONUT_FILES} print/print.c frontend/ctihelp.c frontend/buildsymboltable.c frontend/check_existance.c
        frontend/nodesets/nodeset_child_table.c frontend/nodesets/propagate_attributes.c frontend/unwrap_setexpr.c
        frontend/check_attributes.c frontend/check_product_rules.c frontend/attribute_scheduler/common.h
        frontend/attribute_scheduler/collect_visits.c frontend/attribute_scheduler/collect_visits.h
        frontend/attribute_scheduler/graph.c frontend/attribute_scheduler/graph.h
        frontend/attribute_scheduler/partition_nodes.c frontend/attribute_scheduler/partition_nodes.h
        frontend/attribute_scheduler/queue.c frontend/attribute_scheduler/queue.h
        frontend/attribute_scheduler/fill_visit_stubs.c frontend/attribute_scheduler/fill_visit_stubs.h
        frontend/attribute_scheduler/generate_visits.c frontend/attribute_scheduler/generate_visits.h
        frontend/attribute_scheduler/schedule_attribute_visits.c dot/gen_dot_ag.c dot/gen_dot_ag.h
        dynamic_backend/gen_node_structs.c ${FLEX_DslTokens_OUTPUTS} ${BISON_DslParser_OUTPUTS} gen_helpers/format.c
        dynamic_backend/gen_node_access_funcs.c dynamic_backend/gen_node_access_funcs_error.c
        dynamic_backend/gen_nodeset_access_funcs.c dynamic_backend/gen_nodeset_access_funcs_error.c
        dynamic_backend/gen_node_macros.c dynamic_backend/gen_node_init_funcs.c
        dynamic_backend/gen_node_base_and_union.c dynamic_backend/gen_node_constructor.c dynamic_backend/gen_enums.c
        dynamic_backend/gen_enum_values.c dynamic_backend/gen_trav_v_table.c dynamic_backend/gen_node_trav_table.c
        dynamic_backend/gen_pass_table.c dynamic_backend/gen_trav_table.c dynamic_backend/gen_system_traversals.c
        dynamic_backend/gen_action_header.c dynamic_backend/gen_trav_actions.c dynamic_backend/gen_copy_traversal.c
        dynamic_backend/gen_constructor_call.c dynamic_backend/gen_free_traversal.c
        dynamic_backend/gen_check_traversal.c dynamic_backend/collect_children.c dynamic_backend/collect_children.h
        dynamic_backend/gen_equation_functions.c dynamic_backend/gen_equation_includes.c
        dynamic_backend/gen_user_equations.c dynamic_backend/gen_visit_header.c dynamic_backend/gen_visit_source.c
        dynamic_backend/gen_ag_header.c dynamic_backend/gen_ag_source.c
        phase_driver/gen_action_array.c phase_driver/gen_ids_table.c phase_driver/gen_action_array_header.c
        filesystem/utils.c filesystem/utils.h dynamic_backend/file_switchers.c frontend/reachability.c
        frontend/reachability.h dynamic_backend/gen_trav_structs.c dynamic_backend/gen_trav_data_getters.c
        dynamic_backend/gen_trav_data_ctors.c dynamic_backend/gen_trav_data_union.c
        dynamic_backend/gen_trav_data_ctors_header.c dynamic_backend/gen_trav_data_tables.c dot/gen_dot.c
        frontend/lifetimes/assign_id_to_actions.c frontend/lifetimes/map_id_to_lifetimes.c
        frontend/lifetimes/children_set_mandatory.c sets/literal_insert.c
        sets/set.h sets/set.c sets/literal_lookup.c sets/literal_intersect.c sets/literal_union.c
        sets/literal_difference.c phase_driver/gen_gate_header.c dynamic_backend/gate.c
        dynamic_backend/gen_user_passes.c dynamic_backend/gen_user_traversals.c static/gates.c static/gen_ast_structs.c
        dynamic_backend/gen_helpers.c dynamic_backend/gen_traversal_functions.c frontend/symboltable.c
        frontend/symboltable.h frontend/lifetimes/check_lifetimes.c generator/generator.c
        generator/generator.h ${FILES} gen_helpers.h)

target_compile_options(cocogen PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Wno-unused-function>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:$<$<CONFIG:Debug>:-fsanitize=address>>
        )
target_link_options(cocogen PRIVATE
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:$<$<CONFIG:Debug>:-fsanitize=address>>)

#add_subdirectory(../cunit/CUnit cunit)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")

target_include_directories(cocogen PUBLIC "${CMAKE_CURRENT_LIST_DIR}/ccngen" "${CMAKE_CURRENT_LIST_DIR}" "${PROJECT_SOURCE_DIR}/copra")

target_link_libraries(cocogen PUBLIC coconut::palm)
