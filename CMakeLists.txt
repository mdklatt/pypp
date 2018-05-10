## CMake build file for the pypp library.
##
## This will build the library and its test suite. Compiled binaries will be
## written to build/${CMAKE_BUILD_TYPE}.
##
cmake_minimum_required(VERSION 2.8.12)

project(pypp)

set(PROJECT_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/func.cpp
    ${CMAKE_SOURCE_DIR}/src/path.cpp
    ${CMAKE_SOURCE_DIR}/src/string.cpp
)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -Wall")
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

function(install)
    # Disable install for test directory.
endfunction()
add_subdirectory(test)
