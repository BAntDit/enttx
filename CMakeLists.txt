cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

project(enttx VERSION 3.0.0.0)

option(TESTS "whether needs to build tests" ON)

set(INTERFACE_HEADERS
    src/config.h
    src/entity.h
    src/entityManager.h
    src/baseComponentStorage.h
    src/componentStorage.h
    src/baseSystem.h
    src/systemManager.h
    src/enttx.h
)

set(FULL_PATH_INTERFACE_HEADERS "")
foreach(source ${INTERFACE_HEADERS})
    list(APPEND FULL_PATH_INTERFACE_HEADERS "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${source}>")
endforeach()

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${FULL_PATH_INTERFACE_HEADERS})

target_include_directories(
    ${PROJECT_NAME} INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>"
)

find_package(easy-mp REQUIRED)

if (TESTS)

    find_package(GTest REQUIRED)

    enable_testing()

    add_subdirectory("tests")

endif()

set(ALL_SOURCES_FOR_STYLE_CORRECTION ${INTERFACE_HEADERS})

include(ClangFormat.cmake)
CODE_STYLE_CORRECTION("${ALL_SOURCES_FOR_STYLE_CORRECTION}")

foreach (header ${INTERFACE_HEADERS})
    install(
        FILES ${header}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    )
endforeach()

install(
    TARGETS ${PROJECT_NAME}
    EXPORT  ${PROJECT_NAME}
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Config.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/CMake
)

export(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Config.cmake
)

write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/CMake"
)
