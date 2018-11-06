/// Implementation of the os module.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "sys/stat.h"  // FIXME: not portable
#include "unistd.h"  // FIXME: not portable
#else
#error "os module requires *nix"
#endif

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include "pypp/os.hpp"
#include "pypp/path.hpp"

using std::runtime_error;
using std::strerror;
using std::string;

using namespace pypp;


void os::makedirs(const string& path, mode_t mode, bool exist_ok)
{
    if (not path::isdir(path)) {
        const auto root(path::dirname(path));
        if (not (root.empty() or path::isdir(root))) {
            // Recursively create root directories as needed.
            os::makedirs(root, mode, false);
        }
        if (mkdir(path.c_str(), mode) != 0) {
            if (not (errno == EEXIST or errno == EISDIR)) {
                // Check that this error isn't due to a race condition where
                // the directory was created by another thread/process.
                throw runtime_error(strerror(errno));
            }
        }
    }
    else if (not exist_ok) {
        throw runtime_error("directory exists: " + path);
    }
    return;
}


void os::removedirs(const string& path)
{
    auto pair(path::split(path));
    if (pair.second.empty()) {
        // Handle path with trailing slash.
        pair = path::split(path);
    }
    if (rmdir(pair.second.c_str()) == 0) {
        // Recursively remove as many root directories as possible, stopping
        // silently on failure.
        // TODO: Mimic Python and report failure for initial directory.
        removedirs(pair.first);
    }
    return;
}
