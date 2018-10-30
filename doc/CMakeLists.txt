## CMake configurtion file for project documentation.
##
## This will build Doxygen documents.
## TODO: Sphinx documentation
##
set(DOXYGEN_SKIP_DOT false)
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    SET(DOXYGEN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    configure_file(doxygen/Doxyfile.in ${DOXYGEN_BUILD_DIR}/Doxyfile @ONLY)
    add_custom_target(doxygen
        Doxygen::doxygen Doxyfile
        WORKING_DIRECTORY ${DOXYGEN_BUILD_DIR}
        COMMENT "Generating Doxygen documentation"
        VERBATIM
    )
endif()


# TODO: Define install rules.
