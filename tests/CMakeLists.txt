
cmake_minimum_required(VERSION 3.10)

project("enttx.test")

set(SOURCES
    configuration.h
    test.h
    test.cpp

        testSystem.h)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
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

target_link_libraries(${PROJECT_NAME} gtest gtest_main -pthread)

add_test(NAME enttx-test COMMAND ${PROJECT_NAME})
