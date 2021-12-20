cmake_minimum_required(VERSION 3.14)
project(lib_test LANGUAGES CXX)

message(STATUS "Building in ${CMAKE_CURRENT_BINARY_DIR}")
if(IS_DIRECTORY "${LIB_SOURCE_DIR}")
    add_subdirectory(${LIB_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/lib)
    message(STATUS "Testing library build tree at ${LIB_SOURCE_DIR}")
else()
    find_package(PyPP 0.1.0 REQUIRED)
    get_target_property(path PyPP::pypp LOCATION)
    message(STATUS "Testing installed library at ${path}")
endif()
    

add_executable(libtest libtest.cpp)
target_link_libraries(libtest PRIVATE PyPP::pypp)
