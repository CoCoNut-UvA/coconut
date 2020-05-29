project(ccncc C)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PUBLIC coconut::palm coconut::generated coconut::copra -fsanitize=address -fsanitize=undefined)