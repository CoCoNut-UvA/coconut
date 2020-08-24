target_sources(${PROJECT_NAME}
    PUBLIC
        semantic.h
    PRIVATE
        build_symbol_table.c
        check_existence.c
        eval_setexpr.c
)