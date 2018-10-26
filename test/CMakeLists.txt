## CMake build file for the pypp library test suite.
##
## This will build a Google Test test runner for all contained tests.
##
add_subdirectory(lib/gtest EXCLUDE_FROM_ALL)

add_executable(test_pypp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_path.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_string.cpp
)

target_include_directories(test_pypp
PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(test_pypp
PRIVATE
    ${PROJECT_NAME} gtest gtest_main
)
