/// Test suite for the os module.
///
/// Link all test files with the `gtest_main` library to create a command line
/// test runner.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "sys/stat.h"  // TODO: not portable
#else
#error "os module test suite requires *nix"
#endif

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "pypp/func.hpp"
#include "pypp/os.hpp"
#include "pypp/path.hpp"
#include "pypp/tempfile.hpp"


using pypp::func::in;
using pypp::path::abspath;
using pypp::path::dirname;
using pypp::path::isdir;
using pypp::path::join;
using pypp::path::normpath;
using pypp::path::split;
using pypp::tempfile::TemporaryDirectory;
using std::begin;
using std::end;
using std::runtime_error;
using std::ofstream;
using std::sort;
using std::string;
using std::vector;

using namespace pypp::os;


/// Test the listdir() function.
///
TEST(os, listdir)
{
    const TemporaryDirectory tmpdir;
    const auto fname(join({tmpdir.name, "file"}));
    ofstream stream(fname);
    const auto dname(join({tmpdir.name, "dir"}));
    assert(mkdir(dname.c_str(), 0700) == 0);
    auto names(listdir(tmpdir.name));
    sort(begin(names), end(names));
    ASSERT_EQ(vector<string>({"dir", "file"}), names);
}


/// Test the makedirs() function.
///
TEST(os, makedirs)
{
    static const mode_t mode(0700);
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
        assert(mkdir(path.c_str(), 0700) == 0);
    }
    removedirs(path);  // will also remove tmpdir itself
    const auto pair(split(tmpdir.name));
    ASSERT_TRUE(isdir(pair.first));
    ASSERT_FALSE(isdir(pair.second));
    removedirs(path);  // no op
    ASSERT_TRUE(isdir(pair.first));
}
