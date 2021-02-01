add_test(NAME "GoodDSLfiles" COMMAND runner.sh "$<TARGET_FILE:cocogen>"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
