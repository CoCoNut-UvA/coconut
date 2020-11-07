add_test(NAME "Bad DSL files" COMMAND pytest "--exe=$<TARGET_FILE:myCompiler>"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/fail")