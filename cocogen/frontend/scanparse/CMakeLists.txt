find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(DslParser ${CMAKE_CURRENT_LIST_DIR}/dsl-parser.y ${CMAKE_CURRENT_BINARY_DIR}/dsl-parser.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/dsl-parser.h)
flex_target(DslTokens ${CMAKE_CURRENT_LIST_DIR}/dsl-lexer.l ${CMAKE_CURRENT_BINARY_DIR}/dsl-tokens.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/dsl-lexer.h
)
ADD_FLEX_BISON_DEPENDENCY(DslTokens DslParser)

