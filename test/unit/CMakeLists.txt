include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.1  # TODO: upgrade to v1.11.0
)
FetchContent_MakeAvailable(googletest)
if(IS_DIRECTORY "${googletest_SOURCE_DIR}")
    # CLion doesn't seem to respect EXCLUDE_FROM_ALL, so it will include all
    # googletest targets anyway.
    # https://youtrack.jetbrains.com/issue/CPP-1688
    set_property(DIRECTORY ${googletest_SOURCE_DIR} PROPERTY EXCLUDE_FROM_ALL YES)
endif()

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

include(GoogleTest)
gtest_discover_tests(test_pypp WORKING_DIR ${CMAKE_SOURCE_DIR})
