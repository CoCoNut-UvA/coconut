project(ccncc)

# Important that this one goes before add_executable.
# This is because of linking/generating BISON and FLEX output.

add_executable(${PROJECT_NAME} main.c)

add_subdirectory(inc_core)
add_subdirectory(src_core)
add_subdirectory(inc_generated)
add_subdirectory(src_generated)

target_link_libraries(${PROJECT_NAME} PRIVATE coconut::palm -fsanitize=address -fsanitize=undefined)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)