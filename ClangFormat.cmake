
find_program(
    CLANG_FORMAT
    NAMES "clang-format"
    DOC "Path to clang-format executable"
)

macro(CODE_STYLE_CORRECTION SOURCE_LIST)
    set(FULL_PATH_SOURCE_LIST "")
    foreach(source ${SOURCE_LIST})
        list(APPEND FULL_PATH_SOURCE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
    endforeach()

    set(CODE_STYLE Mozilla)

    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.clang-format")
        set(CODE_STYLE file)
    endif()

    if (CLANG_FORMAT)
        add_custom_target(code-style-format ALL
            COMMAND "${CLANG_FORMAT}" -style=${CODE_STYLE} -i ${FULL_PATH_SOURCE_LIST}
        )
    else(CLANG_FORMAT)
        message("clang-format not found")
    endif()
endmacro()
