message(STATUS "Building test suite")

include(FetchContent)
include(GoogleTest)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.1  # TODO: upgrade to v1.11.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(unit)
