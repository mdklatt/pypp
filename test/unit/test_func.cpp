/// Test suite for the func module.
///
/// Link all test files with the `gtest_main` library to create a command line
/// test runner.
///
#include <iterator>
#include <string>
#include <gtest/gtest.h>
#include "pypp/pypp.hpp"


using namespace pypp::func;

using std::begin;
using std::end;
using std::pair;
using std::string;
using std::tuple;
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


/**
 * Test the enumerate() function.
 */
TEST(func, enumerate) {
    using Vector = vector<pair<ssize_t, char>>;
    auto items(enumerate(string("ab"), 1));
    ASSERT_EQ(Vector(begin(items), end(items)), Vector({{1, 'a'}, {2, 'b'}}));
}


/// Test the in() function.
///
TEST(func, in)
{
    const string seq("abc");
    ASSERT_TRUE(in('a', seq));
    ASSERT_FALSE(in('x', seq));
}


/**
 * Test the zip() function.
 */
TEST(func, zip) {
    const vector<float> it1({1, 2});
    const string it2("ab");
    auto values(zip(it1, it2));
    using V = vector<tuple<float, float>>;
    ASSERT_EQ(V(begin(values), end(values)), V({{1, 'a'}, {2, 'b'}}));
}


/**
 * Test the zip() function for ranges of unequal length.
 */
TEST(func, zip_unequal) {
    const vector<float> it1({1, 2});
    string it2("abc");
    auto values(zip(it1, it2));
    using V = vector<tuple<float, float>>;
    ASSERT_EQ(V(begin(values), end(values)), V({{1, 'a'}, {2, 'b'}}));
}


/// Test fixture for the incremental range() function.
///
/// This is used to group tests and provide common set-up and tear-down code.
/// A new test fixture is created for each test to prevent any side effects
/// between tests. Member variables and methods are injected into each test
/// that uses this fixture.
///
template <typename T>
class IntegralRangeTest: public Test {};
using IntegralTypes = Types<char, int, long, size_t, ssize_t>;
TYPED_TEST_CASE(IntegralRangeTest, IntegralTypes);


/**
 * Test the integral range() function.
 */
TYPED_TEST(IntegralRangeTest, range) {
    auto values(range(TypeParam(2)));
    ASSERT_EQ(vector<TypeParam>(begin(values), end(values)), vector<TypeParam>({0, 1}));
}


/**
 * Test the integral range() function for an empty range.
 */
TYPED_TEST(IntegralRangeTest, range_empty) {
    auto values(range(TypeParam(0)));
    ASSERT_EQ(vector<TypeParam>(begin(values), end(values)), vector<TypeParam>());
}


/**
 * Test fixture for the arithmetic range() function.
 *
 * This is used to group tests and provide common set-up and tear-down code. A
 * new test fixture is created for each test to prevent any side effects
 * between tests. Member variables and methods are injected into each test that
 * uses this fixture.
 */
template <typename T>
class ArithmeticRangeTest: public Test {};
using ArithmeticTypes = Types<int, long, float, double>;  // no unsigned types
TYPED_TEST_CASE(ArithmeticRangeTest, ArithmeticTypes);


/**
 * Test the arithmetic range() function.
 */
TYPED_TEST(ArithmeticRangeTest, no_step) {
    auto values(range<TypeParam>(1, 3));
    ASSERT_EQ(vector<TypeParam>(begin(values), end(values)), vector<TypeParam>({1, 2}));
}


/**
 * Test the arithmetic range() function for an empty range.
 */
TYPED_TEST(ArithmeticRangeTest, no_step_empty) {
    auto values(range<TypeParam>(1, -1));
    ASSERT_EQ(vector<TypeParam>(begin(values), end(values)), vector<TypeParam>());
}


/**
 * Test the arithmetic range() function with a step.
 */
TYPED_TEST(ArithmeticRangeTest, step) {
    auto values(range<TypeParam>(1, 4, 2));
    ASSERT_EQ(vector<TypeParam>(begin(values), end(values)), vector<TypeParam>({1, 3}));
}


/**
 * Test the arithmetic range() function with a negative step.
 */
TYPED_TEST(ArithmeticRangeTest, step_negative) {
    auto values(range<TypeParam>(4, 1, -2));
    ASSERT_EQ(vector<TypeParam>(begin(values), end(values)), vector<TypeParam>({4, 2}));
}


/**
 * Test the arithmetic range() function for an invalid step.
 */
TYPED_TEST(ArithmeticRangeTest, step_zero) {
    ASSERT_THROW(range<TypeParam>(1, 3, 0), std::invalid_argument);
}
