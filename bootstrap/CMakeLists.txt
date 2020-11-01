cmake_minimum_required(VERSION 3.13)

project(example C)

file(GLOB COCONUT_FILES "${CMAKE_CURRENT_LIST_DIR}/ccngen/*.c")

add_subdirectory(coconut/palm build/palm)


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(DslParser src/frontend/scanAndParse/dsl-parser.y ${CMAKE_CURRENT_BINARY_DIR}/dsl-parser.c
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/dsl-parser.h)
flex_target(DslTokens src/frontend/scanAndParse/dsl-lexer.l ${CMAKE_CURRENT_BINARY_DIR}/dsl-tokens.c
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/dsl-lexer.h
        )
ADD_FLEX_BISON_DEPENDENCY(DslTokens DslParser)

add_executable(myCompiler src/main.c src/commandline.c src/frontend/scanAndParse/scan_parse.c
        ${COCONUT_FILES} src/print/print.c src/frontend/buildsymboltable.c src/frontend/check_existance.c
        src/frontend/unwrap_setexpr.c src/dynamic_backend/gen_node_structs.c
        ${FLEX_DslTokens_OUTPUTS} ${BISON_DslParser_OUTPUTS} src/gen_helpers/format.c src/dynamic_backend/gen_node_macros.c
        src/dynamic_backend/gen_node_init_funcs.c src/dynamic_backend/gen_node_base_and_union.c src/dynamic_backend/gen_node_constructor.c
        src/dynamic_backend/gen_enums.c src/dynamic_backend/gen_enum_values.c src/dynamic_backend/gen_trav_v_table.c src/dynamic_backend/gen_node_trav_table.c
        src/dynamic_backend/gen_pass_table.c src/dynamic_backend/gen_trav_table.c src/dynamic_backend/gen_system_traversals.c src/dynamic_backend/gen_action_header.c
        src/dynamic_backend/gen_trav_actions.c src/dynamic_backend/gen_copy_traversal.c src/dynamic_backend/gen_constructor_call.c src/dynamic_backend/gen_free_traversal.c
        src/dynamic_backend/gen_check_traversal.c src/phase_driver/gen_action_array.c src/phase_driver/gen_ids_table.c src/phase_driver/gen_action_array_header.c src/filesystem/gen_files.c
        src/filesystem/gen_files.h src/filesystem/utils.c src/filesystem/utils.h src/dynamic_backend/file_switchers.c src/frontend/reachability.c
        src/frontend/reachability.h src/dynamic_backend/gen_trav_structs.c src/dynamic_backend/gen_trav_data_getters.c src/dynamic_backend/gen_trav_data_ctors.c src/dynamic_backend/gen_trav_data_union.c src/dynamic_backend/gen_trav_data_ctors_header.c src/dynamic_backend/gen_trav_data_tables.c)


target_include_directories(myCompiler PRIVATE "${CMAKE_CURRENT_LIST_DIR}/ccngen" "${CMAKE_CURRENT_LIST_DIR}/src")

target_link_libraries(myCompiler PUBLIC coconut::palm)
