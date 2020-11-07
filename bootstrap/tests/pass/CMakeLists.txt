add_test(NAME "Good DSL files" COMMAND pytest "--exe=$<TARGET_FILE:myCompiler>"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/pass")