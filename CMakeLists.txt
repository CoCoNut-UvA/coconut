cmake_minimum_required(VERSION 3.13)

project(coconut VERSION 0.1 LANGUAGES C)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(ROOT_BIN_DIR ${PROJECT_BINARY_DIR}/bin/)

include(ECMEnableSanitizers)

if (${CMAKE_SOURCE_DIR} EQUAL ${CMAKE_CURRENT_LIST_DIR})
    set(COCONUT_ADD_TESTS "True")
endif()

if (DEFINED ${COCONUT_ADD_TESTS})
    include(CTest)
    enable_testing()
endif()

add_subdirectory(palm/)
add_subdirectory(cocogen/)
add_subdirectory(framework/)

install(TARGETS cocogen DESTINATION bin)