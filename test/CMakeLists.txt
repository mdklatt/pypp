if(PYPP_BUILD_UNIT_TESTS)
    message(STATUS "Enabling unit tests")
    add_subdirectory(unit)
endif()

if(PYPP_BUILD_CMAKE_TESTS)
    # Give find_package() some help locating a Python that's not in the usual
    # system locations, e.g. virtualenv or Conda environments. Python3_ROOT_DIR
    # is apparently used instead of HINTS by FindPython3.
    set(version 3.7)
    execute_process(COMMAND which python OUTPUT_VARIABLE Python3_ROOT_DIR)
    find_package(Python3 ${version} COMPONENTS Interpreter)
    if (NOT Python3_FOUND)
        message(WARNING "Skipping CMake tests; requires Python ${version}+")
        return()
    endif()
    message(STATUS "Enabling CMake tests using ${Python3_EXECUTABLE}")
    set(test_types source install)
    foreach(type ${test_types})
        add_test(
            NAME "cmake_${type}_build"
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/test_cmake.py ${type}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endforeach()
endif()