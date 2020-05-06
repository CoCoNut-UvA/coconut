cmake_minimum_required(VERSION 3.13)

project(example C)

add_executable(ccn-compiler src/main.c src/print.c)

include(../../coconut.cmake)
#set(COCONUT_FLAGS "--verbose" " --consistency-checks")
#coconut_generate(${CMAKE_SOURCE_DIR}/civic.coconut)
coconut_target_generate(ccn-compiler ${CMAKE_SOURCE_DIR}/src/main.ccn)
#target_include_directories(myCompiler PRIVATE ${COCONUT_INCLUDE_DIRS})
#target_link_libraries(myCompiler PRIVATE coconut::palm)
