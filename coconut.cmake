set(COCONUT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

if(NOT DEFINED ${COCONUT_BUILD_DIR})
    set(COCONUT_BUILD_DIR "${COCONUT_ROOT_DIR}/build")
endif()

if(NOT DEFINED ${COCOGEN_DIR})
    set(COCOGEN_DIR "${COCONUT_BUILD_DIR}/cocogen")
endif()

add_subdirectory(${COCONUT_ROOT_DIR}/palm ${CMAKE_CURRENT_BINARY_DIR}/palm)

# We need to check if the DSL file is actually present.

function(cocogen_do_generate DSL_FILE BACKEND)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${DSL_FILE}")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${PROJECT_BINARY_DIR}/generated/src/action_handlers.c")
    if (NOT EXISTS "${COCOGEN_DIR}/cocogen")
        message(FATAL_ERROR "Could not find the cocogen executable in path: ${COCOGEN_DIR}
Maybe you forgot building the coconut project?")
    endif()

    message(STATUS "Generating with cocogen
    ") # Forces newline
    message("${COCOGEN_DIR}")
    message("${PROJECT_BINARY_DIR}")
    execute_process(COMMAND "${COCOGEN_DIR}/cocogen" "--backend" "${BACKEND}" "--gen-user-files" "${DSL_FILE}"
        RESULT_VARIABLE COCOGEN_RET
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
        INPUT_FILE "${DSL_FILE}"
    )
    message("${COCOGEN_RET}")
    if(${COCOGEN_RET})
        message(FATAL_ERROR "cocogen generation failed, stopping CMake.")
    endif()
endfunction()

# IF people want control over interfaces of src and includes.
macro(coconut_generate DSL_FILE)
    cocogen_do_generate("${DSL_FILE}")
    set(COCONUT_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/generated/include/ ${COCONUT_ROOT_DIR}/copra/include/)
    file(GLOB COCONUT_SOURCES ${CMAKE_BINARY_DIR}/generated/src/*.c ${COCONUT_ROOT_DIR}/copra/src/*.c)
endmacro()

function(coconut_target_generate TARGET DSL_FILE BACKEND)
    cocogen_do_generate("${DSL_FILE}" "${BACKEND}")
    file(GLOB COCONUT_SOURCES ${CMAKE_BINARY_DIR}/generated/src/*.c ${COCONUT_ROOT_DIR}/copra/src/*.c)
    target_sources("${TARGET}" PRIVATE "${COCONUT_SOURCES}")
    target_include_directories("${TARGET}" PRIVATE ${CMAKE_BINARY_DIR}/generated/include/ ${COCONUT_ROOT_DIR}/copra/include/)
    target_link_libraries("${TARGET}" PRIVATE coconut::palm)
endfunction()
