## CMake configuration file for the pypp library.
##
## The project defines the pypp::pypp target for use by external projects.
##
cmake_minimum_required(VERSION 3.8)

project(pypp
    VERSION 0.1.0.29  # tweak number is only for dev releases
    LANGUAGES CXX
)


# Define project targets.

add_library(${PROJECT_NAME}
    src/os.cpp
    src/path.cpp
    src/string.cpp
)

target_include_directories(pypp
PUBLIC
    # Interface paths are relative to the installation path, not the current
    # directory.
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(pypp
PRIVATE
    -Wall
    $<$<CXX_COMPILER_ID:GNU>:-pedantic>
)

target_compile_features(pypp
PUBLIC
    cxx_auto_type
)


# Generate package config files.

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PyppConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/PyppConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PyppConfig.cmake
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
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT pypp_targets
    FILE PyppTargets.cmake
    NAMESPACE pypp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
FILES
    ${CMAKE_CURRENT_BINARY_DIR}/PyppConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/PyppConfigVersion.cmake
DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)


# Export targets. If an external project adds this project to its build tree
# via `add_subdirectory()`, then `find_package(pypp)` will work without
# requiring this project to be installed.

export(EXPORT pypp_targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/PyppTargets.cmake
    NAMESPACE pypp::
)
export(PACKAGE pypp)


# Add subdirectories to build.

add_subdirectory(doc)
add_subdirectory(test)
