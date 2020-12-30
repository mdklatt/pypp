cmake_minimum_required(VERSION 3.8)
project(test_pypp VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH $ENV{PYPP_BUILD_DIR})
find_package(PyPP 0.1.0 REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} test_pypp.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
PRIVATE
    PyPP::pypp
)
