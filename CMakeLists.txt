cmake_minimum_required(VERSION 3.14)

project(PyPP
    VERSION 0.2.0.25  # tweak number is only for dev versions
    LANGUAGES CXX
)

option(PYPP_BUILD_DOCS "Build documentation" OFF)
option(PYPP_BUILD_UNIT_TESTS "Build unit tests" OFF)
option(PYPP_BUILD_CMAKE_TESTS "Build CMake integration tests" OFF)
option(PYPP_CMAKE_DEBUG "Display CMake config variables" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PYPP_PACKAGE ${PROJECT_NAME})
set(PYPP_TARGET pypp)


add_subdirectory(src)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${PYPP_PACKAGE}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/${PYPP_PACKAGE}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PYPP_PACKAGE}Config.cmake
    INSTALL_DESTINATION  ${CMAKE_INSTALL_LIBDIR}/cmake/pypp
)


# Define installation rules.

include(GNUInstallDirs)  # TODO: what about MacOS or Windows?
install(TARGETS ${PYPP_TARGET}
    EXPORT pypp_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/pypp
    # Platform-specific headers are defined below.
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PATTERN posix EXCLUDE
    PATTERN win EXCLUDE
)

if(UNIX)
    set(PYPP_PLATFORM posix)
elseif(WIN32)
    set(PYPP_PLATFORM win)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

install(DIRECTORY include/pypp/${PYPP_PLATFORM}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pypp
)

install(EXPORT pypp_targets
    FILE ${PYPP_PACKAGE}Targets.cmake
    NAMESPACE ${PYPP_PACKAGE}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pypp
)

install(
FILES
    ${PROJECT_BINARY_DIR}/${PYPP_PACKAGE}Config.cmake
    ${PROJECT_BINARY_DIR}/${PYPP_PACKAGE}ConfigVersion.cmake
DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/pypp
)


# Export targets. If an external project adds this project to its build tree
# via `add_subdirectory()`, then `find_package(pypp)` will work without
# requiring this project to be installed.

export(EXPORT pypp_targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/${PYPP_PACKAGE}Targets.cmake
    NAMESPACE ${PYPP_PACKAGE}::
)


# Build test suite. This will place the CTest confg at the build root.

if(PYPP_BUILD_UNIT_TESTS OR PYPP_BUILD_CMAKE_TESTS)
    include(CTest)  # calls enable_testing()
    add_subdirectory(test)
endif()


# Build documentation.

if(PYPP_BUILD_DOCS)
    add_subdirectory(doc)
endif()
