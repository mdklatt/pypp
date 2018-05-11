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


/// Pairwise aggregation of two sequences.
///
/// The nth element in the first sequence is paired with the nth element in the
/// sequence. The shortest input sequence determines the length of the result.
///
/// @tparam T1 first value type
/// @tparam T2 second value type
/// @param first first sequence
/// @param second second sequence
/// @return matched pairs
template <typename T1, typename T2>
std::vector<std::pair<T1, T2>> zip(const std::vector<T1>& first, const std::vector<T2>& second)
{
    const auto size(std::min(first.size(), second.size()));
    std::vector<std::pair<T1, T2>> items;
    items.reserve(size);
    for (size_t pos(0); pos < size; ++pos) {
        items.emplace_back(std::make_pair(first[pos], second[pos]));
    }
    return items;
};


}}  // namespace

#endif  // PYPP_FUNC_HPP
