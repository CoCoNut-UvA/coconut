project(ccncc)

# Important that this one goes before add_executable.
# This is because of linking/generating BISON and FLEX output.

add_executable(${PROJECT_NAME} main.c)

add_subdirectory(core/)
add_subdirectory(generated/)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)