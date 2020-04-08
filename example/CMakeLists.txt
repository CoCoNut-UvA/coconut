cmake_minimum_required(VERSION 3.13)

project(example C)

include(../coconut.cmake)
set(COCONUT_FLAGS "--verbose" " --consistency-checks")
coconut_generate(${CMAKE_SOURCE_DIR}/civic.coconut)

#### I Am still working on this part.

# add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/coconut-generated/build/libcoconutgen.a
#     COMMAND ${COCONUT_INSTALL_DIR}/bin/cocogen ${CMAKE_CURRENT_LIST_DIR}/civic.coconut
#     DEPENDS ${CMAKE_CURRENT_LIST_DIR}/civic.coconut
#     COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} -S ${CMAKE_BINARY_DIR}/coconut_generated -B ${CMAKE_BINARY_DIR}/coconut_generated/build
#     COMMAND ${CMAKE_GENERATOR_INSTANCE}
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coconut-generated/build/
# )


# add_custom_target(gen_compiler DEPENDS ${CMAKE_BINARY_DIR}/coconut-generated/build/libcoconutgen.a coconut)

# add_executable(myCompiler main.c ${GENERATED_FILES})
# target_include_directories(myCompiler PRIVATE ${COCONUT_GENERATED_INCLUDE_DIR})
# target_link_libraries(myCompiler PRIVATE coconut::palm)
# add_dependencies(myCompiler gen_compiler)
# target_link_directories(myCompiler PRIVATE ${CMAKE_BINARY_DIR}/coconut-generated/build)
# get_target_property(PALM_INCLUDE_DIR palm INCLUDE_DIRECTORIES)
# message(${PALM_INCLUDE_DIR})
# file(WRITE ${CMAKE_BINARY_DIR}/coconut-generated/CMakeLists.txt
# "cmake_minimum_required(VERSION 3.13)
# project(coconutgen C)
# set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
# "
# )
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/coconut-generated/build/)
