/**
 * Iterator tools.
 *
 * @file
 */
#ifndef PYPP_ITERTOOLS_HPP
#define PYPP_ITERTOOLS_HPP

#include <limits>
#include "generator.hpp"


namespace pypp { namespace itertools {

/**
 * Return an unbounded counter.
 *
 * A std::out_of_range exception is thrown if the counter wraps around due to
 * an overflow.
 *
 * @tparam T: numeric type
 * @param stop: stop value (exclusive)
 * @return count generator
 */
template <typename T=ssize_t>
generator::Counter<T> count(const T& start, const T& step) {
    return generator::Counter<T>(start, step);
}


}}  // pypp::itertools


#endif  // PYPP_ITERTOOLS_HPP
