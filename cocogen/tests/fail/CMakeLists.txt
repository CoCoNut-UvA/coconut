add_test(NAME "BadDSLfiles" COMMAND pytest "--exe=$<TARGET_FILE:cocogen>"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
