/// Test suite for the os module.
///
/// Link all test files with the `gtest_main` library to create a command line
/// test runner.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "sys/stat.h"  // FIXME: not portable
#include "unistd.h"  // FIXME: not portable
#else
#error "os module test suite requires *nix"
#endif

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "pypp/func.hpp"
#include "pypp/os.hpp"
#include "pypp/path.hpp"


using pypp::func::in;
using pypp::path::abspath;
using pypp::path::dirname;
using pypp::path::isdir;
using pypp::path::join;
using pypp::path::normpath;
using pypp::path::split;
using std::begin;
using std::end;
using std::find;
using std::getenv;
using std::remove;
using std::runtime_error;
using std::string;
using std::system;
using std::vector;

using namespace pypp::os;


/// Create a temporary test directory.
///
class TemporaryDirectory
{
public:
    string name;

    TemporaryDirectory()
    {
        string root("/tmp");
        for (const auto& var: {"TMPDIR", "TEMP", "TMP"}) {
            // Attempt to get the system tmp directory from the environment.
            const auto env(getenv(var));
            if (env) {
                root = env;
                break;
            }
        }
        if (not isdir(root)) {
            root = ".";  // default to current directory as last resort
        }
        name = join({root, "tmpXXXXXXXX"});
        mkdtemp(&name[0]);  // name is modified in place
        assert(not name.empty());
    }

    ~TemporaryDirectory()
    {
        // FIXME: This is extremely dangerous.
        const vector<string> prohibited{
            // This is certainly not exhaustive. And it relies on other modules
            // in this library that may not have passed their own test suites
            // yet, so it might might be altogether useless. At best it's
            // (probably) better than nothing.
            "/",
            abspath("."),
        };
        name = abspath(normpath(name));
        if (in(name, prohibited)) {
            // Don't just assert() here because this check always needs to
            // happen.
            throw runtime_error("refusing to delete " + name);
        }
        system(string("rm -rf " + name).c_str());
    }
};


/// Test the makedirs() function.
///
TEST(os, makedirs)
{
    static const mode_t mode(0777);
    TemporaryDirectory tmpdir;
    const auto path(join({tmpdir.name, "abc", "xyz"}));
    makedirs(path, mode);
    ASSERT_TRUE(isdir(path));
    makedirs(path, mode, true);   // exist_ok, no op
    ASSERT_TRUE(isdir(path));
    ASSERT_THROW(makedirs(path), runtime_error);  // not exist_ok
}


/// Test the removedirs() function.
///
TEST(os, removedirs)
{
    static const vector<string> subdirs({"abc", "xyz"});
    TemporaryDirectory tmpdir;
    auto path(tmpdir.name);
    for (const auto& dir: subdirs) {
        // Create subdirectories to remove.
        path = join({path, dir});
        assert(mkdir(path.c_str(), 0777) == 0);
    }
    removedirs(path);  // will also remove tmpdir itself
    const auto pair(split(tmpdir.name));
    ASSERT_TRUE(isdir(pair.first));
    ASSERT_FALSE(isdir(pair.second));
    removedirs(path);  // no op
    ASSERT_TRUE(isdir(pair.first));
}
