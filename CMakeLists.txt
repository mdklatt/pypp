## CMake build file for the pypp library.
##
## This will build the library and its test suite.
##
cmake_minimum_required(VERSION 3.0.2)

project(pypp VERSION 0.1.0.18)  # tweak number is only for dev releases

set(PROJECT_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/path.cpp
    ${CMAKE_SOURCE_DIR}/src/string.cpp
)
set(CMAKE_CXX_STANDARD 11)
add_compile_options("-std=c++11" "-Wall")

add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_include_directories(${PROJECT_NAME}
PUBLIC
    include/
)


function(install)
    # Disable install for test directory.
endfunction()
add_subdirectory(test)
