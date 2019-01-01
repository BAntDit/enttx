cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

project(enttx VERSION 0.0.0.0)

set(PUBLIC_HEADERS
    src/config.h
    src/entity.h
    src/entityManager.h
    src/baseComponentStorage.h
    src/componentStorage.h
    src/baseSystem.h
    src/systemManager.h
)

add_library(enttx INTERFACE)

target_include_directories(
    ${PROJECT_NAME} INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>"
)

find_package(easy-mp REQUIRED)

find_package(Boost REQUIRED)

set(ALL_SOURCES_FOR_STYLE_CORRECTION ${PUBLIC_HEADERS})

include(ClangFormat.cmake)
CODE_STYLE_CORRECTION("${ALL_SOURCES_FOR_STYLE_CORRECTION}")

foreach (header ${PUBLIC_HEADERS})
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
