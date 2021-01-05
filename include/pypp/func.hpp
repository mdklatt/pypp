/// Functional algorithms.
///
/// Based on intrinsic Python functions and operators.
///
/// @file
#ifndef PYPP_FUNC_HPP
#define PYPP_FUNC_HPP

#include <algorithm>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>
#include "generator.hpp"


namespace pypp { namespace func {

/**
 * Evaluate the truth of all values in [first, last).
 *
 * @tparam IT forward iterator type
 * @param first: first position
 * @param last: last position
 * @return true if all items are true or sequence is empty
 */
template <typename IT>
bool all(IT first, IT last) {
    const auto boolean([](const typename IT::value_type& value) {
        return static_cast<bool>(value);
    });
    return std::all_of(first, last, boolean);
}


/**
 * Evaluate the truth of all values in [first, last).
 *
 * @tparam IT forward iterator type
 * @param first: first position
 * @param last: last position
 * @return true if all values are true or sequence is empty
 */
template <typename IT>
bool any(IT first, IT last) {
    const auto boolean([](const typename IT::value_type& value) {
        return static_cast<bool>(value);
    });
    return std::any_of(first, last, boolean);
}


/**
 * Return true if a value is in [first, last).
 *
 * @tparam IT forward iterator type
 * @tparam T search value type (must be comparable to IT::value_type)
 * @param first: first position
 * @param last: last position
 * @return true if value is in sequence
 */
template <typename IT, typename T=typename IT::value_type>
bool in(const T& value, IT first, IT last) {
    return std::find(first, last, value) != last;
}


/**
 * Number each item in a sequence.
 *
 * @tparam IT: forward iterator type
 * @param first: first position
 * @param last: last position
 * @param start: starting count
 * @return (number, item) pairs
 */
template <typename IT>
generator::Enumerator<IT> enumerate(const IT& first, const IT& last, ssize_t start=0) {
    return generator::Enumerator<IT>(first, last, start);
}


/**
 * Return a generator for an elementwise combination of sequences.
 *
 * The nth element in each sequence is combined into a tuple. The shortest
 * input sequence determines the length of the output sequence.
 *
 * @tparam IT1: first iterable type
 * @tparam IT2: second iterable type
 * @tparam T1: first value type
 * @tparam T2: second value type
 * @param it1: first sequence
 * @param it2: second sequence
 * @return: sequence of tuples
 */
template <typename IT1, typename IT2,
    typename T1=typename IT1::value_type, typename T2=typename IT2::value_type>
generator::Zipper<IT1, IT2, T1, T2> zip(const IT1& it1, const IT2& it2) {
    return generator::Zipper<IT1, IT2, T1, T2>(it1, it2);
}


/**
 * Return a generator for the integral range [0, stop). 
 *
 * @tparam T: integral type
 * @param stop: stop value (exclusive)
 * @return range generator
 */
template <typename T>
generator::Range<T> range(const T& stop) {
    return pypp::generator::Range<T>(0, stop, 1);
}


/**
 * Generate an arithmetic range of values using addition.
 *
 * The range [start, stop) is generated using the addition operator with the
 * given step. If the type T2 cannot be initialized to 1, an appropriate step
 * value must be specified.
 *
 * @tparam T: type that supports the required operations
 * @tparam Step: type that is defined for the operation T1 + T2
 * @param start: inclusive start point for the range
 * @param stop: exclusive end point for the range
 * @return range generator
 */
template <typename T, typename Step=T>
generator::Range<T, Step> range(const T& start, const T& stop, const Step& step=1) {
    return pypp::generator::Range<T, Step>(start, stop, step);
}


}}  // pypp::func

#endif  // PYPP_FUNC_HPP
