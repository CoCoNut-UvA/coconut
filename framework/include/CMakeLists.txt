add_subdirectory(core/)
add_subdirectory(generated/)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)