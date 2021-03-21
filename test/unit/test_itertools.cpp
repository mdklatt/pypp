/**
 * Test suite for the itertools module.
 *
 * Link all test files with the `gtest_main` library to create a command line
 * test runner.
 */
#include <iterator>
#include <exception>
#include <limits>
#include <vector>
#include <gtest/gtest.h>
#include "pypp/pypp.hpp"


using namespace pypp::itertools;

using std::begin;
using std::numeric_limits;
using std::vector;


/**
 * Test the itertools::count() function.
 */
TEST(itertools, count) {
    vector<ssize_t> values;
    for (const auto value: count<ssize_t>(-1, 2)) {
        values.emplace_back(value);
        if (values.size() == 3) {
            break;
        }
    }
    ASSERT_EQ(values, vector<ssize_t>({-1, 1, 3}));
}


/**
 * Test the itertools::count() function with a step of 0.
 */
TEST(itertools, count_fixed) {
    vector<ssize_t> values;
    for (const auto value: count<ssize_t>(1, 0)) {
        values.emplace_back(value);
        if (values.size() == 3) {
            break;
        }
    }
    ASSERT_EQ(values, vector<ssize_t>({1, 1, 1}));
}


/**
 * Test the itertools::count() function for an overflow.
 */
TEST(itertools, count_overflow) {
    static const auto start(numeric_limits<ssize_t>::min());
    auto counter(count<ssize_t>(start, -1));
    auto it(begin(counter));
    ASSERT_EQ(start, *it);
    ASSERT_THROW(++it, std::out_of_range);
}
