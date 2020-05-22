project(generated C)

add_library(generated)
add_library(coconut::generated ALIAS generated)

add_subdirectory(include)
add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC coconut::palm  -fsanitize=address -fsanitize=undefined)