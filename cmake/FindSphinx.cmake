## CMake find module for the Sphinx documentation tool.
##
## If Sphinx is found, the module exports the Sphinx::sphinx exectuable target.
##
find_program(SPHINX_EXECUTABLE
NAMES sphinx-build
PATHS
    $ENV{HOME}/.local/bin  # pip installs executables here
DOC "Sphinx documentation tool <http://www.sphinx-doc.org>"
)
mark_as_advanced(SPHINX_EXECUTABLE)
if(SPHINX_EXECUTABLE)
    set(SPHINX_FOUND TRUE)
    execute_process(
        COMMAND "${SPHINX_EXECUTABLE}" --version
        OUTPUT_VARIABLE SPHINX_VERSION
        ERROR_VARIABLE SPHINX_VERSION  # for older versions of Python
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX MATCH "[0-9]+(\.[0-9]+)*" SPHINX_VERSION "${SPHINX_VERSION}")
    if(NOT TARGET Sphinx::sphinx)
        add_executable(Sphinx::sphinx IMPORTED GLOBAL)
        set_target_properties(Sphinx::sphinx
        PROPERTIES
            IMPORTED_LOCATION "${SPHINX_EXECUTABLE}"
        )
    endif()

endif()

find_package_handle_standard_args(
    Sphinx
    REQUIRED_VARS SPHINX_EXECUTABLE
    VERSION_VAR SPHINX_VERSION
    HANDLE_COMPONENTS
)
