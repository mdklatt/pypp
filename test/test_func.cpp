/// Test suite for the func module.
///
/// Link all test files with the `gtest_main` library to create a command line
/// test runner.
///
#include <gtest/gtest.h>
#include "func.hpp"


using namespace pypp::func;

using std::vector;
using std::pair;
using testing::Test;


/// Test the all() function for bools.
///
TEST(func, all_bool)
{
    // TODO: Use type parametrization.
    ASSERT_TRUE(all<bool>({}));
    ASSERT_TRUE(all<bool>({true, true}));
    ASSERT_FALSE(all<bool>({true, false}));
    ASSERT_FALSE(all<bool>({false, false}));
}


/// Test the all() function for ints.
///
TEST(func, all_int)
{
    // TODO: Use type parametrization.
    ASSERT_TRUE(all<int>({}));
    ASSERT_TRUE(all<int>({1, 1}));
    ASSERT_FALSE(all<int>({1, 0}));
    ASSERT_FALSE(all<int>({0, 0}));
}


/// Test the any() function for bools.
///
TEST(func, any_bool)
{
    // TODO: Use type parametrization.
    ASSERT_TRUE(any<bool>({true, true}));
    ASSERT_TRUE(any<bool>({true, false}));
    ASSERT_FALSE(any<bool>({false, false}));
    ASSERT_FALSE(any<bool>({}));
}


/// Test the any() function for ints.
///
TEST(func, any_int)
{
    // TODO: Use type parametrization.
    ASSERT_TRUE(any<int>({1, 1}));
    ASSERT_TRUE(any<int>({1, 0}));
    ASSERT_FALSE(any<int>({0, 0}));
    ASSERT_FALSE(any<int>({}));
}


/// Test the enumerate function.
///
TEST(func, enumerate)
{
    const vector<pair<size_t, char>> pairs{{1, 'a'}, {2, 'b'}};
    const auto enumerated(enumerate<char>({'a', 'b'}, 1));
    ASSERT_EQ(enumerated, pairs);
}


/// Test the zip() function.
///
TEST(func, zip)
{
    const vector<pair<int, char>> pairs{{'a', 1}, {'b', 2}};
    const auto zipped(zip<int, char>({'a', 'b'}, {1, 2, 3}));
    ASSERT_EQ(zipped, pairs);

}
