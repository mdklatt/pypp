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
    static const string stripped("abc \r\t\n");
    ASSERT_EQ(lstrip(stripped), stripped);
    ASSERT_EQ(lstrip("\r\t\n abc \r\t\n"), stripped);
}


/// Test the rstrip() function.
///
TEST(string, rstrip)
{
    static const string stripped("\r\t\n abc");
    ASSERT_EQ(rstrip(stripped), stripped);
    ASSERT_EQ(rstrip("\r\t\n abc \r\t\n"), stripped);
}


/// Test the strip() function.
///
TEST(string, strip)
{
    static const string stripped("abc");
    ASSERT_EQ(strip(stripped), stripped);
    ASSERT_EQ(strip("\r\t\n abc \r\t\n"), stripped);
}
