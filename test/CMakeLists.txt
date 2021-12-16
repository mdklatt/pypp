message(STATUS "Building test suite")

include(FetchContent)
include(GoogleTest)

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

enable_testing()

add_subdirectory(unit)

add_test(
    NAME cmake_source
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/cmake/test_cmake.py source
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
add_test(
    NAME cmake_install
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/cmake/test_cmake.py install
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
