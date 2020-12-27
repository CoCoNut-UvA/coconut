cmake_minimum_required(VERSION 3.13)

project(coconut VERSION 0.1 LANGUAGES C)
enable_testing()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(ROOT_BIN_DIR ${PROJECT_BINARY_DIR}/bin/)

add_subdirectory(palm/)
add_subdirectory(cocogen/)
