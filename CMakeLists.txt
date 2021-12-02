cmake_minimum_required(VERSION 3.14)

project(PyPP
    VERSION 0.2.0.20  # tweak number is only for dev versions
    LANGUAGES CXX
)


# Set language standards.

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Define project targets.

set(PYPP_PACKAGE ${PROJECT_NAME})
set(PYPP_TARGET pypp)

add_library(${PYPP_TARGET}
    src/path.cpp
    src/string.cpp
    $<$<BOOL:${UNIX}>:src/posix/os.cpp>
    $<$<BOOL:${UNIX}>:src/posix/path.cpp>
    $<$<BOOL:${UNIX}>:src/posix/tempfile.cpp>
    $<$<BOOL:${WIN32}>:src/win/path.cpp>
)
add_library(${PYPP_PACKAGE}::${PYPP_TARGET} ALIAS ${PYPP_TARGET})

target_include_directories(${PYPP_TARGET}
PUBLIC
    # Interface paths are relative to the installation path, not the current
    # directory.
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(${PYPP_TARGET}
PRIVATE
    -Wall
    $<$<CXX_COMPILER_ID:GNU>:-pedantic>
)

target_compile_features(${PYPP_TARGET}
PUBLIC
    cxx_auto_type
)


# Generate header file. Output is written to the same file for every build
# type, so beware of content that is dependent on the build configuration
# Lock the output file to prevent a race condition.

file(LOCK ${CMAKE_SOURCE_DIR}/include/pypp/pypp.hpp)
configure_file(src/pypp.hpp.in ${CMAKE_SOURCE_DIR}/include/pypp/pypp.hpp @ONLY)


# Generate package config files.

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PYPP_PACKAGE}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/${PYPP_PACKAGE}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PYPP_PACKAGE}Config.cmake
INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/pypp
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

if(${UNIX})
    set(PYPP_PLATFORM posix)
elseif(${WIN32})
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
    ${CMAKE_CURRENT_BINARY_DIR}/${PYPP_PACKAGE}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PYPP_PACKAGE}ConfigVersion.cmake
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
export(PACKAGE ${PYPP_PACKAGE})


# Build test suite.

option(PYPP_BUILD_TESTS "Build test suite" OFF)

if(PYPP_BUILD_TESTS)
    message(STATUS "Building test suite")
    enable_testing()
    include(CTest)
    include(FetchContent)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.1  # TODO: upgrade to v1.11.0
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(test_pypp
        test/unit/test_func.cpp
        test/unit/test_itertools.cpp
        test/unit/test_os.cpp
        test/unit/test_path.cpp
        test/unit/test_string.cpp
        test/unit/test_tempfile.cpp
    )

    target_link_libraries(test_pypp
    PRIVATE
        PyPP::pypp gtest gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(test_pypp)
    add_test(
        NAME unit
        COMMAND test_pypp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()


# Build documentation.

option(PYPP_BUILD_DOCS "Build documentation" OFF)
if(PYPP_BUILD_DOCS)
    add_subdirectory(doc)
endif()
