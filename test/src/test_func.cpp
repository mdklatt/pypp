/// Test suite for the func module.
///
/// Link all test files with the `gtest_main` library to create a command line
/// test runner.
///
#include <string>
#include <gtest/gtest.h>
#include "pypp/func.hpp"


using namespace pypp::func;

using std::pair;
using std::string;
using std::vector;
using testing::Test;
using testing::Types;


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


/// Test the enumerate() function.
///
TEST(func, enumerate)
{
    const vector<pair<size_t, char>> pairs{{1, 'a'}, {2, 'b'}};
    const auto enumerated(enumerate<char>({'a', 'b'}, 1));
    ASSERT_EQ(enumerated, pairs);
}


/// Test the in() function.
///
TEST(func, in)
{
    const string seq("abc");
    ASSERT_TRUE(in('a', seq));
    ASSERT_FALSE(in('x', seq));
}


/// Test the zip() function.
///
TEST(func, zip)
{
    const vector<pair<int, char>> pairs{{'a', 1}, {'b', 2}};
    const auto zipped(zip<int, char>({'a', 'b'}, {1, 2, 3}));
    ASSERT_EQ(zipped, pairs);

}


/// Test fixture for the incremental range() function.
///
/// This is used to group tests and provide common set-up and tear-down code.
/// A new test fixture is created for each test to prevent any side effects
/// between tests. Member variables and methods are injected into each test
/// that uses this fixture.
///
template <typename T>
class IncrementalRangeTest: public Test {};

using IncrementalTypes = Types<char, int, long, size_t, ssize_t>;
TYPED_TEST_CASE(IncrementalRangeTest, IncrementalTypes);


/// Test the incremental range() function.
///
TYPED_TEST(IncrementalRangeTest, range)
{
    ASSERT_EQ(range(TypeParam{}), vector<TypeParam>({}));
    ASSERT_EQ(range<TypeParam>(2), vector<TypeParam>({0, 1}));
}


/// Test fixture for the arithmetic range() function.
///
/// This is used to group tests and provide common set-up and tear-down code.
/// A new test fixture is created for each test to prevent any side effects
/// between tests. Member variables and methods are injected into each test
/// that uses this fixture.
///
template <typename T>
class ArithmeticRangeTest: public Test {};

using ArithmeticTypes = Types<int, long, float, double>;  // no unsigned types
TYPED_TEST_CASE(ArithmeticRangeTest, ArithmeticTypes);


/// Test the numeric range() function.
///
TYPED_TEST(ArithmeticRangeTest, range)
{
    ASSERT_EQ(range<TypeParam>(1, -1), vector<TypeParam>({}));
    ASSERT_EQ(range<TypeParam>(1, 3), vector<TypeParam>({1, 2}));
    ASSERT_EQ(range<TypeParam>(1, 4, 2), vector<TypeParam>({1, 3}));
    ASSERT_EQ(range<TypeParam>(4, 1, 2), vector<TypeParam>({}));
    ASSERT_EQ(range<TypeParam>(4, 1, -2), vector<TypeParam>({4, 2}));
    ASSERT_THROW(range<TypeParam>(1, 3, 0), std::invalid_argument);
}
