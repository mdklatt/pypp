/// POSIX implementation of the 'os' module.
///
#include "sys/stat.h"
#include "dirent.h"
#include "unistd.h"
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include "pypp/func.hpp"
#include "pypp/os.hpp"
#include "pypp/path.hpp"


using std::runtime_error;
using std::strerror;
using std::string;
using std::vector;

using namespace pypp;


vector<string> os::listdir(const string& path)
{
    static const vector<string> special({".", ".."});
    vector<string> names;
    errno = 0;  // POSIX requires this to be thread-safe
    auto dir(opendir(path.c_str()));
    if (errno != 0) {
        throw runtime_error(string(strerror(errno)) + ": " + path);
    }
    dirent* entry;
    while ((entry = readdir(dir))) {
        if (not func::in(entry->d_name, special)) {
            names.emplace_back(entry->d_name);
        }
    }
    closedir(dir);
    if (errno != 0) {
        throw runtime_error(strerror(errno));
    }
    return names;
}


void os::makedirs(const string& path, mode_t mode, bool exist_ok)
{
    if (not path::isdir(path)) {
        const auto root(path::dirname(path));
        if (not (root.empty() or path::isdir(root))) {
            // Recursively create root directories as needed.
            makedirs(root, mode, false);
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
