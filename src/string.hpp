/// Interface for the string module.
///
/// @file
#ifndef PYPP_STRING_HPP
#define PYPP_STRING_HPP

#include <string>


namespace pypp {

/// Convert a string to lower case.
///
/// @param str input string
/// @return modified string
std::string lower(std::string str);


/// Convert a string to upper case.
///
/// @param str input string
/// @return modified string
std::string upper(std::string str);


/// Remove leading whitespace from a string.
///
/// @param str input string
/// @return modified string
std::string lstrip(const std::string& str);


/// Remove trailing whitespace from a string.
///
/// @param str input string
/// @return modified string
std::string rstrip(const std::string& str);


/// Remove whitespace from both ends of a string.
///
/// @param str input string
/// @return modified string
std::string strip(const std::string& str);

}

#endif  // PYPP_STRING_HPP 
