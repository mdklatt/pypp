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
/// @return lower case string
std::string lower(std::string str);


/// Convert a string to upper case.
///
/// @param str input string
/// @return uppder case string
std::string upper(std::string str);

}

#endif  // PYPP_STRING_HPP 
