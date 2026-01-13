cmake_minimum_required(VERSION 3.10)
project(
  cvector
  VERSION 1.0.0
  LANGUAGES C CXX
  DESCRIPTION "A simple vector library for C that can store any type."
)

add_library(cvector vec.c)
set_target_properties(cvector PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "vec.h"
)

include(GNUInstallDirs)
target_include_directories(
  cvector PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS cvector
    EXPORT cvectorTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT cvectorTargets
    FILE cvectorTargets.cmake
    NAMESPACE cvector::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cvector
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cvectorConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
    VERSION ${PROJECT_VERSION}
)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/cvectorConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cvectorConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cvector
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cvectorConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cvectorConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cvector
)
