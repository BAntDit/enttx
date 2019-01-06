cmake_minimum_required(VERSION 3.10)

project(enttx.example VERSION 0.0.0.0)

set(SOURCE_FILES
    components/transform.h
    components/camera.h
    components/directionalLight.h
    app.h
    example.h
    options.h
    options.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(glm REQUIRED)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_FORCE_RADIANS)
target_link_libraries(${PROJECT_NAME} glm)

find_package(Boost REQUIRED COMPONENTS
    program_options
    system
)

target_link_libraries(${PROJECT_NAME} Boost::boost
    Boost::program_options
    Boost::system
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}

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
