/// Miscellaneous functions.
///
/// Based on intrinsic Python functions.
///
/// @file
#ifndef PYPP_FUNC_HPP
#define PYPP_FUNC_HPP

#include <algorithm>
#include <utility>
#include <vector>


namespace pypp { namespace func {

/// Evaluate the truth of all items in a sequence.
///
/// @param items input items
/// @return true if all items are true or sequence is empty
bool all(const std::vector<bool>& items);


/// Evaluate the truth of any item in a sequence.
///
/// @param items input items
/// @return true if any item is true and sequence is not empty
bool any(const std::vector<bool>& items);


/// Index each item in a sequence.
///
/// Each input item is paired with its position in the sequence.
///
/// @tparam T sequence value type
/// @param items input sequence
/// @param start starting index
/// @return (index, item) pairs
template <typename T>
std::vector<std::pair<size_t, T>> enumerate(const std::vector<T>& items, ssize_t start=0)
{
    std::vector<std::pair<size_t, T>> pairs;
    pairs.reserve(items.size());
    ssize_t pos(start);
    for (const auto& item: items) {
        pairs.emplace_back(std::make_pair(pos++, item));
    }
    return pairs;
};


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
};


}}  // namespace

#endif  // PYPP_FUNC_HPP
