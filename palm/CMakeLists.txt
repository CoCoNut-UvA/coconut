project(palm C)

add_library(${PROJECT_NAME})
add_library(coconut::palm ALIAS palm)

add_subdirectory(src/)
add_subdirectory(include/palm/)

target_include_directories(${PROJECT_NAME}
    PUBLIC include/
)

# Link math library
if (${CMAKE_C_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()


add_executable(palm_tests tests.c)

target_link_libraries(palm_tests PRIVATE coconut::palm)