## CMake configuration file for the pypp library.
##
## The project defines the pypp::pypp target for use by external projects.
##
cmake_minimum_required(VERSION 3.8)

project(pypp
    VERSION 0.2.0.2
    LANGUAGES CXX
)


# Define project targets.

add_library(${CMAKE_PROJECT_NAME}
    src/string.cpp
    $<${UNIX}:src/posix/os.cpp>
    $<${UNIX}:src/posix/path.cpp>
    $<${UNIX}:src/posix/tempfile.cpp>
)
add_library(pypp::pypp ALIAS pypp)

target_include_directories(${CMAKE_PROJECT_NAME}
PUBLIC
    # Interface paths are relative to the installation path, not the current
    # directory.
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(${CMAKE_PROJECT_NAME}
PRIVATE
    -Wall
    $<$<CXX_COMPILER_ID:GNU>:-pedantic>
)

target_compile_features(${CMAKE_PROJECT_NAME}
PUBLIC
    cxx_auto_type
)


# Generate header file. Output is written to the same file for every build
# type, so beware of content that is dependent on the build configuration
# Lock the output file to prevent a race condition.

message(STATUS ${CMAKE_SOURCE_DIR})
file(LOCK ${CMAKE_SOURCE_DIR}/include/pypp/pypp.hpp GUARD FILE)
configure_file(src/pypp.hpp.in ${CMAKE_SOURCE_DIR}/include/pypp/pypp.hpp @ONLY)


# Generate package config files.

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/pypp-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/pypp-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/pypp-config.cmake
INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)


# Define installation rules.

include(GNUInstallDirs)  # TODO: what about MacOS or Windows?
install(TARGETS pypp
    EXPORT pypp_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/
    # TODO: Exclude unused platform-specific directories.
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT pypp_targets
    FILE PyppTargets.cmake
    NAMESPACE pypp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pypp-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/pypp-config-version.cmake
DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)


# Export targets. If an external project adds this project to its build tree
# via `add_subdirectory()`, then `find_package(pypp)` will work without
# requiring this project to be installed.

export(EXPORT pypp_targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/pypp-targets.cmake
    NAMESPACE pypp::
)
export(PACKAGE pypp)


# Add subdirectories to build.

add_subdirectory(doc)
add_subdirectory(test/unit)
