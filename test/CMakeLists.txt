## CMake build file for the pypp library test suite.
##
## This will build a Google Test test runner for all contained tests.
##
add_subdirectory(vendor/gtest EXCLUDE_FROM_ALL)

add_executable(test_pypp
    src/test_func.cpp
    src/test_path.cpp
    src/test_string.cpp
)

target_include_directories(test_pypp
PRIVATE
    src
)

target_link_libraries(test_pypp
PRIVATE
    ${PROJECT_NAME} gtest gtest_main
)
