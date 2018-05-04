/// Test suite for the path module.
///
/// Link all test files with the `gtest_main` library to create a command-line 
/// test runner.
///
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "path.hpp"


using namespace pypp::path;

using testing::Test;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;


/// Test the join() function.
///
TEST(path, join)
{
    // These test cases correspond with the observed Python behavior of
    // os.path.join(), not its documented behavior.
    ASSERT_EQ(join({"/abc/"}), "/abc/");
    ASSERT_EQ(join({"/abc", "xyz"}), "/abc/xyz");
    ASSERT_EQ(join({"abc/", "xyz/"}), "abc/xyz/");
    ASSERT_EQ(join({"/abc/", "/xyz/"}), "/xyz/");
    ASSERT_EQ(join({"/abc//", "xyz", ""}), "/abc//xyz/");
    ASSERT_EQ(join({""}), "");
}


/// Test the split() function.
///
TEST(path, split)
{
    ASSERT_EQ(split("//abc"), make_pair(string("//"), string("abc")));
    ASSERT_EQ(split("/abc/xyz"), make_pair(string("/abc"), string("xyz")));
    ASSERT_EQ(split("abc//xyz"), make_pair(string("abc"), string("xyz")));
    ASSERT_EQ(split("abc"), make_pair(string(""), string("abc")));
    ASSERT_EQ(split(""), make_pair(string(""), string("")));
}


/// Test the dirname() function.
///
TEST(path, dirname)
{
    ASSERT_EQ(dirname("//abc"), "//");
    ASSERT_EQ(dirname("/abc/xyz"), "/abc");
    ASSERT_EQ(dirname("abc//xyz"), "abc");
    ASSERT_EQ(dirname("abc"), "");
    ASSERT_EQ(dirname(""), "");
}


/// Test the basename() function.
///
TEST(path, basename)
{
    // The explicit string casts for the expected values are needed to avoid
    // spurious failures on Linux with both clang and gcc (i.e. Travis CI).
    // Without a cast, Google Test appears to test for pointer equality instead
    // of string equality. Tests pass without explicit casts on MacOS with
    // clang. Note that casts are only needed in cases where there is no
    // directory component. Running with valgrind doesn't reveal any obvious
    // issues with the basename() function.
    ASSERT_EQ(basename("//abc"), string("abc"));
    ASSERT_EQ(basename("/abc/xyz"), string("xyz"));
    ASSERT_EQ(basename("abc//xyz"), string("xyz"));
    ASSERT_EQ(basename("abc"), "abc");
    ASSERT_EQ(basename(""), "");
}
