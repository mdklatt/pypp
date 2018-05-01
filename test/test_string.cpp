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
    ASSERT_EQ(lower("abc"), "abc");
    ASSERT_EQ(lower("ABC"), "abc");
}


/// Test the upper() function.
TEST(string, upper)
{
    ASSERT_EQ(upper("ABC"), "ABC");
    ASSERT_EQ(upper("abc"), "ABC");
}
