target_sources(${PROJECT_NAME}
    PUBLIC
        format.h
        gen-functions.h
    PRIVATE
        gen-functions.c
        format.c
)