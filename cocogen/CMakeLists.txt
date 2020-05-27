project(cocogen)

# Important that this one goes before add_executable.
# This is because of linking/generating BISON and FLEX output.
include(frontend/scanparse/CMakeLists.txt)

add_executable(${PROJECT_NAME} main.c commandline.c commandline.h ${FLEX_DslTokens_OUTPUTS} ${BISON_DslParser_OUTPUTS})

add_subdirectory(frontend/)
add_subdirectory(pretty/)
add_subdirectory(filegen/)
add_subdirectory(phase-driver/)
add_subdirectory(dynamic_backend/)
add_subdirectory(typed_backend/)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE coconut::palm)

# Link math library
if (${CMAKE_C_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

if(DEFINED ${COCUNT_ENABLE_TESTING})
    add_subdirectory(tests)
endif()
