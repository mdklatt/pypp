/// Test suite for the string module.
///
/// Link all test files with the `gtest_main` library to create a command-line 
/// test runner.
///
#include <string>
#include <gtest/gtest.h>
#include "string.hpp"


using testing::Test;
using testing::Types;
using std::string;

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
    static const string empty;
    static const string stripped("abc" + space);
    ASSERT_EQ(lstrip(stripped), stripped);
    ASSERT_EQ(lstrip(space + "abc" + space), stripped);
    ASSERT_EQ(lstrip(space), empty);
    ASSERT_EQ(lstrip(empty), empty);
}


/// Test the rstrip() function.
///
TEST(string, rstrip)
{
    static const string space("\r\t\n ");
    static const string empty;
    static const string stripped(space + "abc");
    ASSERT_EQ(rstrip(stripped), stripped);
    ASSERT_EQ(rstrip(space + "abc" + space), stripped);
    ASSERT_EQ(rstrip(space), empty);
    ASSERT_EQ(rstrip(empty), empty);
}


/// Test the strip() function.
///
TEST(string, strip)
{
    static const string space("\r\t\n ");
    static const string empty;
    static const string stripped("abc");
    ASSERT_EQ(strip(stripped), stripped);
    ASSERT_EQ(strip(space + "abc" + space), stripped);
    ASSERT_EQ(strip(space), empty);
    ASSERT_EQ(strip(empty), empty);
}
