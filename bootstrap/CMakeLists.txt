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
        ${FLEX_DslTokens_OUTPUTS} ${BISON_DslParser_OUTPUTS} src/gen_helpers/format.c src/dynamic_backend/gen_node_macros.c src/dynamic_backend/gen_node_init_funcs.c)


target_include_directories(myCompiler PRIVATE "${CMAKE_CURRENT_LIST_DIR}/ccngen" "${CMAKE_CURRENT_LIST_DIR}/src")

target_link_libraries(myCompiler PUBLIC coconut::palm)
