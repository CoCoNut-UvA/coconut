project(copra C)

add_library(copra)
add_library(coconut::copra ALIAS copra)

target_sources(${PROJECT_NAME}
    PRIVATE
        action_handling.c
        break_inspect_points.c
        consist_checks.c
        cycle.c
        error.c
        globals.c
        internal_phase_functions.c
        memory.c
        phase_driver.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${COCONUT_INCLUDE_DIR} ./)
target_link_libraries(${PROJECT_NAME} PUBLIC coconut::palm)