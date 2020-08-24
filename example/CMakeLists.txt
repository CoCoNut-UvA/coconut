cmake_minimum_required(VERSION 3.13)

project(example C)

file(GLOB COCONUT_FILES "${CMAKE_CURRENT_LIST_DIR}/ccngen/*.c")

add_subdirectory(../palm/ build/palm)

add_executable(myCompiler main.c ${COCONUT_FILES})

target_include_directories(myCompiler PRIVATE "${CMAKE_CURRENT_LIST_DIR}/ccngen/")

target_link_libraries(myCompiler PUBLIC coconut::palm)
