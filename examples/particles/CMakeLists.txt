
cmake_minimum_required(VERSION 3.15)

project("enttx.examples.particles")

set(SOURCES
    glad/glad.c
    src/view.cpp
    src/main.cpp
)

set(PUBLIC_HEADERS
    src/view.h
    src/controller.h
)

add_executable(
    ${PROJECT_NAME}
    ${PUBLIC_HEADERS}
    ${SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF

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

find_package(Boost REQUIRED COMPONENTS system)

target_link_libraries(${PROJECT_NAME}
    Boost::boost
    Boost::system
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL)

target_link_libraries(${PROJECT_NAME} SDL2)
