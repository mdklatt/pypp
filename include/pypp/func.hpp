/// Functional algorithms.
///
/// Based on intrinsic Python functions and operators.
///
/// @file
#ifndef PYPP_FUNC_HPP
#define PYPP_FUNC_HPP

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>


namespace pypp { namespace func {

/// Evaluate the truth of all items in a sequence.
///
/// @tparam T sequence value type (must be bool convertible)
/// @param seq input sequence
/// @return true if all items are true or sequence is empty
template <typename T>
bool all(const std::vector<T>& seq)
{
    const auto boolean([](const T& x){ return static_cast<bool>(x); });
    return std::all_of(std::begin(seq), std::end(seq), boolean);
}


/// Evaluate the truth of any item in a sequence.
///
/// @tparam T sequence value type (must be bool convertible)
/// @param seq input sequence
/// @return true if any item is true and sequence is not empty
template <typename T>
bool any(const std::vector<T>& seq)
{
    const auto boolean([](const T& x){ return static_cast<bool>(x); });
    return std::any_of(std::begin(seq), std::end(seq), boolean);
}


/// Index each item in a sequence.
///
/// Each input item is paired with its index in the sequence.
///
/// @tparam T sequence value type
/// @param seq input sequence
/// @param start starting index
/// @return (index, item) pairs
template <typename T>
std::vector<std::pair<size_t, T>> enumerate(const std::vector<T>& seq, ssize_t start=0)
{
    std::vector<std::pair<size_t, T>> items;
    items.reserve(seq.size());
    ssize_t pos(start);
    for (const auto& item: seq) {
        items.emplace_back(std::make_pair(pos++, item));
    }
    return items;
}


/// Return true if a value is in a sequence.
///
/// The value to search for must be comparable to the sequence value type.
///
/// @tparam T value type (must be comparable with sequence value type)
/// @tparam C sequence container type
/// @param value search value
/// @param cont search sequence
/// @return true if value is in sequence
template <typename T, class C>
bool in(const T& value, const C& seq)
{
    const auto end(std::end(seq));
    return std::find(begin(seq), end, value) != end;
}


/// Pairwise aggregation of two sequences.
///
/// The nth element in the first sequence is paired with the nth element in the
/// sequence. The shortest input sequence determines the length of the result.
///
/// @tparam T1 first sequence value type
/// @tparam T2 second sequence value type
/// @param first first sequence
/// @param second second sequence
/// @return matched pairs
template <typename T1, typename T2>
std::vector<std::pair<T1, T2>> zip(const std::vector<T1>& first, const std::vector<T2>& second)
{
    const auto size(std::min(first.size(), second.size()));
    std::vector<std::pair<T1, T2>> pairs;
    pairs.reserve(size);
    for (size_t pos(0); pos < size; ++pos) {
        pairs.emplace_back(std::make_pair(first[pos], second[pos]));
    }
    return pairs;
}


/// Generate an incremental range.
///
/// For a zero-initialized start value of T, the range [start, stop) is
/// generated using the preincrement operator. Unlike with the std::iota()
/// algorithm, the size of the range does not need to be known beforehand.
///
/// @tparam T type that supports the required operations
/// @param stop exclusive end point for the range
/// @return range of values
template <typename T>
std::vector<T> range(const T& stop)
{
    // T is not required to implement differencing, so there is no way to
    // predetermine the size of the vector here.
    std::vector<T> values;
    for (T value{}; value != stop; ++value) {
        values.emplace_back(value);
    }
    return values;
}


/// Generate an arithmetic range of values using addition.
///
/// The range [start, stop) is generated using the addition operator with the
/// given step. If the type T2 cannot be initialized to 1, an appropriate step
/// value must be specified.
///
/// @tparam T1 type that supports the required operations
/// @tparam T2 type that is defined for the operation T1 + T2
/// @param start inclusive start point for the range
/// @param stop exclusive end point for the range
/// @return range of values
template <typename T1, typename T2=T1>
std::vector<T1> range(const T1& start, const T1& stop, const T2& step=1)
{
    // T is not required to implement differencing, so there is no way to
    // predetermine the size of the vector here.
    const T2 zero{};
    if (step == zero) {
        throw std::invalid_argument("step must be nonzero");
    }
    std::vector<T1> values;
    if (start < stop) {
        if (step < zero) {
            return values;
        }
        for (T1 value(start); value < stop; value += step) {
            values.emplace_back(value);
        }
    }
    else {
        if (step > zero) {
            return values;
        }
        for (T1 value(start); value > stop; value += step) {
            values.emplace_back(value);
        }
    }
    return values;
}

}}  // namespace

#endif  // PYPP_FUNC_HPP
