cmake_minimum_required(VERSION 3.10)

project(enttx VERSION 0.0.0.0)

set(PUBLIC_HEADERS
    src/config.h
    src/entity.h
    src/EntityManager.h
    src/baseComponentStorage.h
    src/componentStorage.h
    src/baseSystem.h
    src/systemManager.h
)

set(SOURCE_FILES
    src/entity.cpp
)

add_library(enttx SHARED
    ${PUBLIC_HEADERS}
    ${SOURCE_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}

    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF

    COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -fPIC"

    DEBUG_POSTFIX _d
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -pedantic
        -Wall
        -Wextra
        -Wfatal-errors
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE
        /Wall
    )
endif()

find_package(easy-mp REQUIRED)

find_package(Boost REQUIRED)

set(ALL_SOURCES_FOR_STYLE_CORRECTION ${PUBLIC_HEADERS} ${SOURCE_FILES})

include(ClangFormat.cmake)
CODE_STYLE_CORRECTION("${ALL_SOURCES_FOR_STYLE_CORRECTION}")
