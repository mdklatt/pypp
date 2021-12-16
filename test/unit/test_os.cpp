/**
 * Test suite for the os module.
 *
 * Link all test files with the `gtest_main` library to create a command line
 * test runner.
 */
#include <unistd.h>  // chdir
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "pypp/libtest.hpp"


using pypp::path::abspath;
using pypp::path::dirname;
using pypp::path::isdir;
using pypp::path::join;
using pypp::path::split;
using pypp::tempfile::TemporaryDirectory;
using std::begin;
using std::end;
using std::runtime_error;
using std::fstream;
using std::string;
using std::set;
using std::vector;
using testing::Test;

using namespace pypp::os;


/**
 *  Test the os::getcwd() function.
 */
TEST(os, getcwd) {
     char buffer[FILENAME_MAX];
     getcwd(buffer, sizeof(buffer));
     ASSERT_EQ(getcwd(), string(buffer));
}


/**
 * Test fixture for the chdir() function.
 *
 * Public and protected member variables and methods are injected into each
 * test that uses this fixture.
 */
class ChdirTest: public Test
{
public:
    ChdirTest(): cwd(getcwd()) {}

    ~ChdirTest() override {
        ::chdir(cwd.c_str());
    }

private:
    const string cwd;
};


/**
 * Test the os::chdir() function.
 */
TEST_F(ChdirTest, chdir) {
    const auto path(abspath(dirname(__FILE__)));
    chdir(path);
    ASSERT_EQ(path, getcwd());
}


/**
 * Test the os::listdir() function.
 */
TEST(os, listdir) {
    const TemporaryDirectory tmpdir;
    const auto fname(join({tmpdir.name(), "file"}));
    fstream stream(fname, fstream::out);  // create empty file
    makedirs(join({tmpdir.name(), "dir"}), 0700);
    const auto paths(listdir(tmpdir.name()));
    set<string> items(begin(paths), end(paths));  // guaranteed ordering
    ASSERT_EQ(set<string>({"dir", "file"}), items);
    ASSERT_THROW(listdir(fname), runtime_error);
}


/**
 * Test the os::makedirs() function.
 */
TEST(os, makedirs) {
    static const mode_t mode(0700);
    TemporaryDirectory tmpdir;
    const auto path(join({tmpdir.name(), "abc", "xyz"}));
    makedirs(path, mode);
    ASSERT_TRUE(isdir(path));
    makedirs(path, mode, true);   // exist_ok, no op
    ASSERT_TRUE(isdir(path));
    ASSERT_THROW(makedirs(path), runtime_error);  // not exist_ok
}


/**
 * Test the os::removedirs() function.
 */
TEST(os, removedirs) {
    static const vector<string> subdirs({"abc", "xyz"});
    TemporaryDirectory tmpdir;
    auto path(tmpdir.name());
    for (const auto& dir: subdirs) {
        // Create subdirectories to remove.
        makedirs(join({path, dir}), 0700);
    }
    removedirs(path);  // will also remove tmpdir itself
    const auto pair(split(tmpdir.name()));
    ASSERT_TRUE(isdir(pair.first));
    ASSERT_FALSE(isdir(pair.second));
    removedirs(path);  // no op
    ASSERT_TRUE(isdir(pair.first));
}
