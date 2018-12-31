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

add_library(enttx INTERFACE)

find_package(easy-mp REQUIRED)

find_package(Boost REQUIRED)

set(ALL_SOURCES_FOR_STYLE_CORRECTION ${PUBLIC_HEADERS})

include(ClangFormat.cmake)
CODE_STYLE_CORRECTION("${ALL_SOURCES_FOR_STYLE_CORRECTION}")
