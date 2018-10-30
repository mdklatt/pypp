## CMake configuration file for project documentation.
##
## Doxygen and/or Sphinx documentation can be built and installed if the
## necessary tools are installed.
##


# Define documentation targets.

set(DOXYGEN_SKIP_DOT false)
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    configure_file(doxygen/Doxyfile.in ${_BUILD_DIR}/Doxyfile @ONLY)
    add_custom_target(doxygen
        Doxygen::doxygen Doxyfile
        WORKING_DIRECTORY ${_BUILD_DIR}
        COMMENT "Generating Doxygen documentation"
        VERBATIM
    )
    unset(_BUILD_DIR)
endif()


list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(Sphinx QUIET)
list(REMOVE_AT CMAKE_MODULE_PATH -1)
if (SPHINX_FOUND)
    set(_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
    configure_file(sphinx/conf.py.in ${_BUILD_DIR}/conf.py @ONLY)
    add_custom_target(sphinx
        Sphinx::sphinx -c . -b html ${PROJECT_SOURCE_DIR}/doc/sphinx html
        WORKING_DIRECTORY ${_BUILD_DIR}
        COMMENT "Generating Sphinx documentation"
        VERBATIM
    )
    unset(_BUILD_DIR)
endif()


add_custom_target(docs
DEPENDS
    $<$<BOOL:${DOXYGEN_FOUND}>:doxygen>
    $<$<BOOL:${SPHINX_FOUND}>:sphinx>
)


# TODO: Define install rules.
