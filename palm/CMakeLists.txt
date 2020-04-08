project(palm C)

add_library(palm)
add_library(coconut::palm ALIAS palm)

add_subdirectory(src/)
add_subdirectory(include/lib/)

target_include_directories(${PROJECT_NAME}
    PUBLIC include/
)

