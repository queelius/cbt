cmake_minimum_required(VERSION 3.14)
project(cbt VERSION 1.1.0 LANGUAGES CXX)

# C++17 is required for CBT library
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(CBT_BUILD_EXAMPLES "Build CBT examples" ON)
option(CBT_BUILD_TESTS "Build CBT tests" ON)
option(CBT_BUILD_DOCS "Build CBT documentation" OFF)

# CBT is a header-only library
add_library(cbt INTERFACE)
add_library(cbt::cbt ALIAS cbt)

target_include_directories(cbt INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Compiler features
target_compile_features(cbt INTERFACE cxx_std_17)

# Examples
if(CBT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(CBT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS cbt
    EXPORT cbt-targets
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT cbt-targets
    FILE cbt-targets.cmake
    NAMESPACE cbt::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cbt
)

# Create package configuration files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cbt-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cbt-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cbt
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cbt-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cbt-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cbt-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cbt
)

# Export for use from build tree
export(EXPORT cbt-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/cbt-targets.cmake
    NAMESPACE cbt::
)