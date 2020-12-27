add_test(NAME "GoodDSLfiles" COMMAND pytest "--exe=$<TARGET_FILE:cocogen>"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
