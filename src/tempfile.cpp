/// Implementation of the tempfile module.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "unistd.h"  // TODO: not portable
#else
#error "tempfile module requires *nix"
#endif

#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>
#include "pypp/func.hpp"
#include "pypp/os.hpp"
#include "pypp/path.hpp"
#include "pypp/tempfile.hpp"


using pypp::func::in;
using pypp::os::listdir;
using pypp::path::abspath;
using pypp::path::isdir;
using pypp::path::join;
using pypp::path::Path;
using std::getenv;
using std::remove;
using std::runtime_error;
using std::string;
using std::vector;

using namespace pypp::tempfile;


std::string pypp::tempfile::gettempdir()
{
    // TODO: Verify that the directory is actually user-writable.
    static const vector<string> vars({"TMPDIR", "TEMP", "TMP"});
    static const vector<string> dirs({"/tmp", "/var/tmp", "/usr/tmp", "."});
    static string tmpdir;
    if (tmpdir.empty()) {
        // Calculate the result the first time the function is called.
        for (const auto& var: vars) {
            // Attempt to get the directory from the environment.
            const auto env(getenv(var.c_str()));
            if (env and isdir(env)) {
                tmpdir = env;
                break;
            }
        }
        if (tmpdir.empty()) {
            for (const auto& dir: dirs) {
                // Attempt to use a standard directory, or CWD as last resort.
                if (isdir(dir)) {
                    tmpdir = dir;
                    break;
                }
            }
        }
        assert(not tmpdir.empty());
    }
    return abspath(tmpdir);  // expands CWD dynamically if needed
}


TemporaryDirectory::TemporaryDirectory(const string& prefix, string dir)
{
    // The Python counterpart supports an optional directory suffix, but this
    // is not supported by all versions of mkdtemp(), so it is not implemented
    // here.
    if (dir.empty()) {
        dir = gettempdir();
    }
    string tmpdir(join({dir, prefix + "XXXXXXXX"}));
    if (not mkdtemp(&tmpdir[0])) {
        throw runtime_error(strerror(errno));
    }
    path = Path(tmpdir);
}


std::string TemporaryDirectory::name() const
{
    return string(path);
}


void TemporaryDirectory::cleanup() const
{
    rmtree(path);
    return;
}


TemporaryDirectory::~TemporaryDirectory()
{
    rmtree(path, true);
}


void TemporaryDirectory::rmtree(const Path& root, bool delroot)
{
    for (const auto& item: root.iterdir()) {
        if (not item.is_dir()) {
            item.unlink();
        }
        else {
            rmtree(item, true);
        }
    }
    if (delroot) {
        root.rmdir();
    }
    return;
}
