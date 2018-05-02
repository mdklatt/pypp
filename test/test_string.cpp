/// Test suite for the string module.
///
/// Link all test files with the `gtest_main` library to create a command-line 
/// test runner.
///
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "string.hpp"


using testing::Test;
using testing::Types;
using std::string;
using std::vector;

using namespace pypp;


/// Test the lower() function.
///
TEST(string, lower)
{
    static const string lcase("abc");
    ASSERT_EQ(lower(lcase), lcase);
    ASSERT_EQ(lower("ABC"), lcase);
}


/// Test the upper() function.
TEST(string, upper)
{
    static const string ucase("ABC");
    ASSERT_EQ(upper(ucase), ucase);
    ASSERT_EQ(upper("abc"), ucase);
}


/// Test the lstrip() function.
///
TEST(string, lstrip)
{
    static const string space("\r\t\n ");
    static const string stripped("abc" + space);
    ASSERT_EQ(lstrip(stripped), stripped);
    ASSERT_EQ(lstrip(space + "abc" + space), stripped);
    ASSERT_EQ(lstrip(space), "");
    ASSERT_EQ(lstrip(""), "");
}


/// Test the rstrip() function.
///
TEST(string, rstrip)
{
    static const string space("\r\t\n ");
    static const string stripped(space + "abc");
    ASSERT_EQ(rstrip(stripped), stripped);
    ASSERT_EQ(rstrip(space + "abc" + space), stripped);
    ASSERT_EQ(rstrip(space), "");
    ASSERT_EQ(rstrip(""), "");
}


/// Test the strip() function.
///
TEST(string, strip)
{
    static const string space("\r\t\n ");
    static const string stripped("abc");
    ASSERT_EQ(strip(stripped), stripped);
    ASSERT_EQ(strip(space + "abc" + space), stripped);
    ASSERT_EQ(strip(space), "");
    ASSERT_EQ(strip(""), "");
}


/// Test the join() function.
///
TEST(string, join)
{
    static const string joined("a,b,c");
    ASSERT_EQ(join({"a", "b", "c", }, ","), joined);
}


/// Test the split() function with whitespace.
///
TEST(string, split_space)
{
    static const vector<string> items({"abc", "xyz", "123"});
    static const string joined(" \rabc\t xyz \n123 \n");
    ASSERT_EQ(split(joined), items);
    ASSERT_EQ(split(joined, "", 0), vector<string>({"abc\t xyz \n123 \n"}));
    ASSERT_EQ(split(joined, "", 1), vector<string>({"abc", "xyz \n123 \n"}));
    ASSERT_EQ(split(""), vector<string>());
}


/// Test the split() function with a delimiter.
///
TEST(string, split_delim)
{
    static const string sep("00");
    static const string joined("00abc0000xyz00");
    static const vector<string> items({"", "abc", "", "xyz", ""});
    ASSERT_EQ(split(joined, sep), items);
    ASSERT_EQ(split(joined, sep, 0), vector<string>{joined});
    ASSERT_EQ(split(joined, sep, 2), vector<string>({"", "abc", "00xyz00"}));
    ASSERT_EQ(split(sep, sep), vector<string>({"", ""}));
    ASSERT_EQ(split("", sep), vector<string>({""}));
}
