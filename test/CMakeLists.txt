## CMake build file for the pypp library test suite.
##
## This will build a Google Test test runner for all contained tests.
##
project(test_pypp)

add_subdirectory(lib/gtest)

set(PROJECT_TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_path.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_string.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${PROJECT_TEST_FILES})
target_link_libraries(${PROJECT_NAME} pypp gtest gtest_main)
