/// Miscellaneous functions.
///
/// Based on intrinsic Python functions.
///
/// @file
#ifndef PYPP_FUNC_HPP
#define PYPP_FUNC_HPP

#include <vector>


namespace pypp { namespace func {

/// Evaluate the truth of all items in a sequence.
///
/// This returns as soon as the first false item is encountered.
///
/// @param items input items
/// @return true if all items are true or sequence is empty
bool all(const std::vector<bool>& items);


/// Evaluate the truth of any item in a sequence.
///
/// @param items input items
/// @return true if any item is true and sequence is not empty
bool any(const std::vector<bool>& items);


}}  // namespace

#endif  // PYPP_FUNC_HPP
