/**
 * Common file path manipulations.
 *
 * This combines functionality from the Python os.path and pathlib modules.
 * @file
 */
#ifndef PYPP_PATH_HPP
#define PYPP_PATH_HPP

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))

#include "pypp/posix/path.hpp"
namespace pypp { namespace path {

using PurePath = pypp::path::PurePosixPath;
using Path = pypp::path::PosixPath;

}}
#else
#error "path module requires POSIX"
#endif

#endif  // PYPP_PATH_HPP
