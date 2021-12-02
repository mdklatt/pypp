/**
 * Windows-specific implementations of the 'path' module.
 */
#include <string>
#include "pypp/path.hpp"

using std::string;

using namespace pypp;
using path::PureWindowsPath;


const char path::SEP(PYPP_WINDOWS_SEP);
