add_executable(test_pypp
    test_func.cpp
    test_itertools.cpp
    test_os.cpp
    test_path.cpp
    test_string.cpp
    test_tempfile.cpp
)

target_link_libraries(test_pypp
PRIVATE
    PyPP::pypp gtest gtest_main
)

gtest_discover_tests(test_pypp)
add_test(
    NAME unit_tests
    COMMAND test_pypp
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)