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


/// Test the all() function.
///
TEST(func, all)
{
    ASSERT_TRUE(all({}));
    ASSERT_TRUE(all({true, true}));
    ASSERT_FALSE(all({true, false}));
    ASSERT_FALSE(all({false, false}));
}


/// Test the any() function.
///
TEST(func, any)
{
    ASSERT_TRUE(any({true, true}));
    ASSERT_TRUE(any({true, false}));
    ASSERT_FALSE(any({false, false}));
    ASSERT_FALSE(any({}));
}


/// Test the zip() function.
///
TEST(func, zip)
{
    const vector<pair<int, char>> zipped{{0, 'a'}, {1, 'b'}};
    const auto result(zip<int, char>({0, 1}, {'a', 'b', 'c'}));
    ASSERT_EQ(result, zipped);

}
