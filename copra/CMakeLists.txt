project(copra C)

add_library(copra)
add_library(coconut::copra ALIAS copra)

add_subdirectory(include)
add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC coconut::palm  -fsanitize=address -fsanitize=undefined)