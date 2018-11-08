/// Operating system tools.
///
/// @file
#ifndef PYPP_OS_HPP
#define PYPP_OS_HPP

#include <string>
#include <vector>


namespace pypp { namespace os {

/// List the names in a directory.
///
/// Special entries (e.g. "." and "..") are ignored.
///
/// @param path directory path
/// @return
std::vector<std::string> listdir(const std::string& path=".");


/// Recursively make a new directory.
///
/// @param path
/// @param mode
/// @param exist_ok
void makedirs(const std::string& path, mode_t mode=0777, bool exist_ok=false);


/// Recursively remove empty directories.
///
/// @param path
void removedirs(const std::string& path);

}}

#endif  // PYPP_OS_HPP
