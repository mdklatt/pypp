## CMake build file for the pypp library test suite.
##
## This will build a Google Test test runner for all contained tests.
##
project(test_pypp)

add_subdirectory(lib/gtest)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} test_module.cpp)
target_link_libraries(${PROJECT_NAME} pypp gtest gtest_main)
