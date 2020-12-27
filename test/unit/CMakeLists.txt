## CMake configuration file for the project test suite.
##
## This will build a Google Test test runner for all contained tests.
##
add_subdirectory(vendor/gtest EXCLUDE_FROM_ALL)

add_executable(test_pypp
    test_func.cpp
    test_os.cpp
    test_path.cpp
    test_string.cpp
    test_tempfile.cpp
)

target_link_libraries(test_pypp
PRIVATE
    pypp::pypp gtest gtest_main
)
