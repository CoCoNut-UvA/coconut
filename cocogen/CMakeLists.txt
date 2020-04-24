project(cocogen)

# Important that this one goes before add_executable.
# This is because of linking/generating BISON and FLEX output.
include(frontend/scanparse/CMakeLists.txt)

add_executable(${PROJECT_NAME} main.c commandline.c commandline.h ${FLEX_DslTokens_OUTPUTS} ${BISON_DslParser_OUTPUTS})

add_subdirectory(frontend/)
add_subdirectory(pretty/)
add_subdirectory(filegen/)
add_subdirectory(framework/)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE coconut::palm)

if(DEFINED ${COCUNT_ENABLE_TESTING})
    add_subdirectory(tests)
endif()