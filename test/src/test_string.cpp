/// Test suite for the string module.
///
/// Link all test files with the `gtest_main` library to create a command-line 
/// test runner.
///
#include <stdexcept>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "pypp/string.hpp"


using testing::Test;
using testing::Types;
using std::string;
using std::vector;

using namespace pypp::str;


/// Test the whitespace constant.
///
TEST(string, whitespace)
{
    ASSERT_FALSE(whitespace.empty());  // make sure it's defined
}


/// Test the lower() function for a char.
///
TEST(string, lower_char)
{
    ASSERT_EQ(lower('a'), 'a');
    ASSERT_EQ(lower('A'), 'a');
}


/// Test the lower() function for a string.
///
TEST(string, lower_str)
{
    ASSERT_EQ(lower("abc"), "abc");
    ASSERT_EQ(lower("ABC"), "abc");
}


/// Test the upper() function for a char.
///
TEST(string, upper_char)
{
    ASSERT_EQ(upper('A'), 'A');
    ASSERT_EQ(upper('a'), 'A');
}


/// Test the upper() function for a string.
TEST(string, upper_str)
{
    ASSERT_EQ(upper("ABC"), "ABC");
    ASSERT_EQ(upper("abc"), "ABC");
}


/// Test the lstrip() function for whitespace.
///
TEST(string, lstrip)
{
    static const string stripped("abc" + whitespace);
    ASSERT_EQ(lstrip(stripped), stripped);
    ASSERT_EQ(lstrip(whitespace + "abc" + whitespace), stripped);
    ASSERT_EQ(lstrip(whitespace), "");
    ASSERT_EQ(lstrip(""), "");
}


/// Test the lstrip() function for a set of characters.
///
TEST(string, lstrip_chars)
{
    static const string chars("012");
    static const string stripped("abc001122");
    ASSERT_EQ(lstrip(stripped, chars), stripped);
    ASSERT_EQ(lstrip("001122abc001122", chars), stripped);
    ASSERT_EQ(lstrip("001122", chars), "");
    ASSERT_EQ(lstrip("", chars), "");
}


/// Test the rstrip() function for whitespace.
///
TEST(string, rstrip)
{
    static const string stripped(whitespace + "abc");
    ASSERT_EQ(rstrip(stripped), stripped);
    ASSERT_EQ(rstrip(whitespace + "abc" + whitespace), stripped);
    ASSERT_EQ(rstrip(whitespace), "");
    ASSERT_EQ(rstrip(""), "");
}


/// Test the rstrip() function for a set of characters.
///
TEST(string, rstrip_chars)
{
    static const string chars("012");
    static const string stripped("001122abc");
    ASSERT_EQ(rstrip(stripped, chars), stripped);
    ASSERT_EQ(rstrip("001122abc001122", chars), stripped);
    ASSERT_EQ(rstrip("001122", chars), "");
    ASSERT_EQ(rstrip("", chars), "");
}


/// Test the strip() function for whitespace.
///
TEST(string, strip)
{
    static const string stripped("abc");
    ASSERT_EQ(strip(stripped), stripped);
    ASSERT_EQ(strip(whitespace + "abc" + whitespace), stripped);
    ASSERT_EQ(strip(whitespace), "");
    ASSERT_EQ(strip(""), "");
}


/// Test the strip() function for a set of characters.
///
TEST(string, strip_chars)
{
    static const string chars("012");
    static const string stripped("abc");
    ASSERT_EQ(strip(stripped, chars), stripped);
    ASSERT_EQ(strip("001122abc001122", chars), stripped);
    ASSERT_EQ(strip("001122", chars), "");
    ASSERT_EQ(strip("", chars), "");
}


/// Test the join() function for a string separator.
///
TEST(string, join_str)
{
    static const string sep(", ");
    ASSERT_EQ(join(sep, {"a", "b", "c",}), "a, b, c");
    ASSERT_EQ(join(sep, {"a", "b", ",c,"}), "a, b, ,c,");
}


/// Test the join() function for a char separator.
///
TEST(string, join_char)
{
    static const char sep(',');
    ASSERT_EQ(join(sep, {"a", "b", "c",}), "a,b,c");
    ASSERT_EQ(join(sep, {"a", "b", ",c,"}), "a,b,,c,");
}


/// Test the split() function for whitespace.
///
TEST(string, split)
{
    static const vector<string> items({"abc", "xyz", "123"});
    static const string joined(" \rabc\t xyz \n123 \n");
    ASSERT_EQ(split(joined), items);
    ASSERT_EQ(split(joined, 0), vector<string>({"abc\t xyz \n123 \n"}));
    ASSERT_EQ(split(joined, 1), vector<string>({"abc", "xyz \n123 \n"}));
    ASSERT_EQ(split(""), vector<string>());
}


/// Test the split() function for a separator.
///
TEST(string, split_sep)
{
    static const string joined(", abc, , xyz, ");
    static const vector<string> items({"", "abc", "", "xyz", ""});
    static const string sep(", ");  // test multi-char separators
    ASSERT_EQ(split(joined, sep), items);
    ASSERT_EQ(split(joined, sep, 0), vector<string>{joined});
    ASSERT_EQ(split(joined, sep, 2), vector<string>({"", "abc", ", xyz, "}));
    ASSERT_EQ(split(sep, sep), vector<string>({"", ""}));
    ASSERT_EQ(split("", sep), vector<string>({""}));
    ASSERT_THROW(split("", ""), std::invalid_argument);
}


/// Test the startswith() function.
///
TEST(string, startswith)
{
    ASSERT_TRUE(startswith("abc", "ab"));
    ASSERT_TRUE(startswith("abc", "abc"));
    ASSERT_FALSE(startswith("abc", "xyz"));
    ASSERT_FALSE(startswith("abc", "abcabc"));  // test prefix longer than str
}


/// Test the endswith() function.
///
TEST(string, endswith)
{
    ASSERT_TRUE(endswith("abc", "bc"));
    ASSERT_TRUE(endswith("abc", "abc"));
    ASSERT_FALSE(endswith("abc", "xyz"));
    ASSERT_FALSE(endswith("abc", "abcabc"));  // test suffix longer than str
}


/// Test the replace() function.
///
TEST(string, replace)
{
    ASSERT_EQ(replace("", "abcabc", "xyz"), "");
    ASSERT_EQ(replace("abc", "", "xyz"), "xyzaxyzbxyzcxyz");
    ASSERT_EQ(replace("abcabc", "abc", ""), "");
    ASSERT_EQ(replace("abcabc", "abc", "xyz"), "xyzxyz");
    ASSERT_EQ(replace("abc", "abc", "xyzxyz"), "xyzxyz");
    ASSERT_EQ(replace("abcabc", "abc", "xyz", 0), "abcabc");
    ASSERT_EQ(replace("abcabc", "abc", "xyz", 1), "xyzabc");
}


/// Test the center() function.
///
TEST(string, center)
{
    ASSERT_EQ(center("abc", 2), "abc");
    ASSERT_EQ(center("abc", 4), "abc ");
    ASSERT_EQ(center("abc", 5), " abc ");
    ASSERT_EQ(center("abc", 5, 'x'), "xabcx");
}
