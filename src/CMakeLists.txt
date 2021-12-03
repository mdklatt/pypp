add_library(${PYPP_TARGET}
    path.cpp
    string.cpp
    $<$<BOOL:${UNIX}>:posix/os.cpp>
    $<$<BOOL:${UNIX}>:posix/path.cpp>
    $<$<BOOL:${UNIX}>:posix/tempfile.cpp>
    $<$<BOOL:${WIN32}>:win/path.cpp>
)
add_library(${PYPP_PACKAGE}::${PYPP_TARGET} ALIAS ${PYPP_TARGET})

target_include_directories(${PYPP_TARGET}
PUBLIC
    # Interface paths are relative to the installation path, not the current
    # directory.
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_options(${PYPP_TARGET}
PRIVATE
    -Wall
    $<$<CXX_COMPILER_ID:GNU>:-pedantic>
)

if(PYPP_CMAKE_DEBUG)
    set(props COMPILE_DEFINITIONS COMPILE_FLAGS COMPILE_OPTIONS INCLUDE_DIRECTORIES INTERFACE_INCLUDE_DIRECTORIES BINARY_DIR)
    foreach(prop ${props})
        get_target_property(value ${PYPP_TARGET} ${prop})
        message(STATUS "${PYPP_TARGET}: ${prop}=${value}")
    endforeach()
endif()


# Generate header file. Output is written to the same file for every build
# type, so beware of content that is dependent on the build configuration
# Lock the output file to prevent a race condition.

file(LOCK ${PROJECT_SOURCE_DIR}/include/pypp/pypp.hpp)
configure_file(pypp.hpp.in ${PROJECT_SOURCE_DIR}/include/pypp/pypp.hpp @ONLY)
