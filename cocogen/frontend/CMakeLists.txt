add_subdirectory(semantic)
add_subdirectory(reachability)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})