## CMake build file for the pypp library.
##
## This will build the library and its test suite.
##
cmake_minimum_required(VERSION 3.8)

project(pypp VERSION 0.1.0.19)  # tweak number is only for dev releases

add_library(${PROJECT_NAME}
    src/path.cpp
    src/string.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    # TODO: Why do build paths have to be absolute?
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(${PROJECT_NAME}
PRIVATE
    -Wall
    $<$<CXX_COMPILER_ID:GNU>:-pedantic>
)

target_compile_features(${PROJECT_NAME}
PUBLIC
    cxx_auto_type
)


# Install library.

include(GNUInstallDirs)  # TODO: what about MacOS or Windows?
install(TARGETS pypp
    EXPORT pypp-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


# Build tests

add_subdirectory(test)
