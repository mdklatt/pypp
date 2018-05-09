/// Common file path manipulations.
///
/// Combines functionality from Python os.path and pathlib (TODO).
///
/// @file
#ifndef PYPP_PATH_HPP
#define PYPP_PATH_HPP

#include <string>
#include <utility>
#include <vector>


namespace pypp { namespace path {

/// Path separator.
///
/// In the current implementation this is not OS-specific, but works for both
/// *nix and Windows.
///
extern const std::string sep;


/// Join path segments into a complete path.
///
/// Use an empty string as the last to segment to ensure that the path ends in
/// a trailing separator.
///
/// @param parts individual path parts
/// @return complete path
std::string join(const std::vector<std::string>& parts);


/// Split a path into directory and name components.
///
/// If the path has a trailing separator, the pathname component will be empty.
/// Calling join() on the resulting segments will return an equivalent (but not
/// necessarily identical) path.
///
/// @param path path
/// @return (root, name) pair
std::pair<std::string, std::string> split(const std::string& path);


/// Get the directory component from a path.
///
/// This is identical to the first value of the result returned by split().
///
/// @param path path
/// @return directory component
std::string dirname(const std::string& path);


/// Get the name component from a path.
///
/// This is identical to the second value of the result returned by split().
///
/// @param path complete path
/// @return name component
std::string basename(const std::string& path);


/// Normalize a path.
///
/// @param path path
/// @return normalized path
std::string normpath(const std::string& path);

}}  // namespace

#endif  // PYPP_PATH_HPP
