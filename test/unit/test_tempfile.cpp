/// Test suite for the tempfile module.
///
/// Link all test files with the `gtest_main` library to create a command line
/// test runner.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "sys/stat.h"  // TODO: not portable
#else
#error "tempfile module test suite requires *nix"
#endif

#include <cassert>
#include <fstream>
#include <string>

#include <gtest/gtest.h>
#include "pypp/path.hpp"
#include "pypp/string.hpp"
#include "pypp/tempfile.hpp"

using pypp::path::basename;
using pypp::path::dirname;
using pypp::path::isdir;
using pypp::path::isfile;
using pypp::path::join;
using pypp::str::endswith;
using pypp::str::startswith;
using std::ofstream;
using std::string;
using testing::Test;

using namespace pypp::tempfile;


/// Test the gettempdir() function.
///
TEST(tempfile, gettempdir)
{
    // TODO: This is just a basic smoke test.
    ASSERT_TRUE(isdir(gettempdir()));
}


/// Test the TemporaryDirectory constructor.
///
TEST(TemporaryDirectoryTest, ctor)
{
    // This also tests the 'name' attribute.
    const TemporaryDirectory tmpdir("abc");
    ASSERT_TRUE(isdir(tmpdir.name()));
    ASSERT_TRUE(startswith(basename(tmpdir.name()), "abc"));
    ASSERT_EQ(gettempdir(), dirname(tmpdir.name()));
}


/// Test the TemporaryDirectory constructor with a custom directory.
///
TEST(TemporaryDirectoryTest, ctor_dir)
{
    const TemporaryDirectory root;
    const TemporaryDirectory tmpdir("tmp", root.name());
    ASSERT_TRUE(isdir(tmpdir.name()));
    ASSERT_EQ(root.name(), dirname(tmpdir.name()));
}


/// Test the TemporaryFile::cleanup() method.
///
TEST(TemporaryDirectoryTest, cleanup)
{
    const TemporaryDirectory tmpdir;
    const auto dname(join({tmpdir.name(), "dir"}));
    assert(mkdir(dname.c_str(), 0700) == 0);
    const auto fname(join({dname, "file"}));
    ofstream stream(fname);
    stream.close();
    tmpdir.cleanup();
    ASSERT_TRUE(isdir(tmpdir.name()));
    ASSERT_FALSE(isdir(dname));
}


/// Test the TemporaryDirectory destructor.
///
TEST(TemporaryDirectoryTest, dtor)
{
    const auto tmpdir(new TemporaryDirectory());
    const auto name(tmpdir->name());
    delete tmpdir;
    ASSERT_FALSE(isdir(name));
}