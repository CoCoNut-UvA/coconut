project(cocogen)

# Important that this one goes before add_executable.
# This is because of linking/generating BISON and FLEX output.
include(frontend/scanparse/CMakeLists.txt)

add_executable(${PROJECT_NAME} main.c ${FLEX_DslTokens_OUTPUTS} ${BISON_DslParser_OUTPUTS})

add_subdirectory(ast/)
add_subdirectory(frontend/)
add_subdirectory(format/)
add_subdirectory(backends/)
add_subdirectory(filehandling/)
add_subdirectory(phase_driver/)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE coconut::palm)

# Link math library
if (${CMAKE_C_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()