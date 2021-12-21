if(PYPP_BUILD_UNIT_TESTS)
    message(STATUS "Enabling unit tests")
    add_subdirectory(unit)
endif()

if(PYPP_BUILD_CMAKE_TESTS)
    find_package(Python COMPONENTS Interpreter)
    if(NOT (Python_VERSION_MAJOR EQUAL 3 AND Python_VERSION_MINOR GREATER_EQUAL 7))
        message(SEND_ERROR "Python 3.7+ required for CMake tests")
        return()
    endif()
    message(STATUS "Enabling CMake integration tests")
    set(test_types source install)
    foreach(type ${test_types})
        add_test(
            NAME "cmake_${type}_build"
            COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/test_cmake.py ${type}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endforeach()
endif()
