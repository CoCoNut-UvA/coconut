cmake_minimum_required(VERSION 3.13)

project(example C)

add_executable(myCompiler main.c pass_createast.c trav_simpleprint.c)

include(../coconut.cmake)
#set(COCONUT_FLAGS "--verbose" " --consistency-checks")
#coconut_generate(${CMAKE_SOURCE_DIR}/civic.coconut)
coconut_target_generate(myCompiler ${CMAKE_SOURCE_DIR}/simple.ccn dynamic)
#target_include_directories(myCompiler PRIVATE ${COCONUT_INCLUDE_DIRS})
#target_link_libraries(myCompiler PRIVATE coconut::palm)
