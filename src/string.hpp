/// Interface for the string module.
///
/// @file
#ifndef PYPP_STRING_HPP
#define PYPP_STRING_HPP

#include <string>
#include <vector>


namespace pypp {

/// List of whitespace characters.
///
/// In the current implementation this is not locale-aware.
///
extern const std::string whitespace;


/// Convert a string to lower case.
///
/// In the current implementation this is not locale-aware.
///
/// @param str input string
/// @return modified string
std::string lower(std::string str);


/// Convert a string to upper case.
///
/// In the current implementation this is not locale-aware.
///
/// @param str input string
/// @return modified string
std::string upper(std::string str);


/// Remove leading characters from a string.
///
/// @param str input string
/// @param chars set of characters to remove
/// @return modified string
std::string lstrip(const std::string& str, const std::string& chars=whitespace);


/// Remove trailing characters from a string.
///
/// @param str input string
/// @param chars set of characters to remove
/// @return modified string
std::string rstrip(const std::string& str, const std::string& chars=whitespace);


/// Remove leading and trailing characters from of a string.
///
/// In the current implementation this is not locale-aware.
///
/// @param str input string
/// @param chars set of characters to remove
/// @return modified string
std::string strip(const std::string& str, const std::string& chars=whitespace);


/// Join strings using a separator.
///
std::string join(const std::string& sep, const std::vector<std::string>& items);


/// Split a string.
///
/// If no separator is given, the string is split on whitespace. Leading and
/// trailing whitespace is ignored. If a separator is given, all occurrences of
/// the separator are significant, and will generate empty strings as items
/// as appropriate.
///
/// If `maxsplit` is non-negative, a maximum of that many splits will be
/// performed. The last item will contain the remainder of the string
/// regardless of any whitespace or separators it contains.
///
/// @param str string to split
/// @param sep separator to split on
/// @param maxsplit maximum number of splits to perform
/// @return split items
std::vector<std::string> split(const std::string& str, const std::string& sep="", ssize_t maxsplit=-1);

}  // namespace

#endif  // PYPP_STRING_HPP 
