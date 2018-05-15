/// Common string utilities.
///
/// This combines functionality from the Python str type and the string module.
///
/// @file
#ifndef PYPP_STRING_HPP
#define PYPP_STRING_HPP

#include <string>
#include <vector>


namespace pypp { namespace str {

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


/// Split a string on whitespace.
///
/// Leading and trailing whitespace is ignored.
///
/// If `maxsplit` is non-negative, a maximum of that many splits will be
/// performed. The last item will contain the remainder of the string
/// regardless of any whitespace it contains.
///
/// @param str string to split
/// @param maxsplit maximum number of splits to perform
/// @return split items
std::vector<std::string> split(const std::string& str, ssize_t maxsplit=-1);


/// Split a string on a separator.
///
/// All occurrences of the separator are significant, and will generate empty
/// strings as items as appropriate.
///
/// If `maxsplit` is non-negative, a maximum of that many splits will be
/// performed. The last item will contain the remainder of the string
/// regardless of any separators it contains.
///
/// @param str string to split
/// @param sep separator to split on
/// @param maxsplit maximum number of splits to perform
/// @return split items
std::vector<std::string> split(const std::string& str, const std::string& sep, ssize_t maxsplit=-1);


/// Determine if a string starts with a prefix.
///
/// Unlike the Python version, this does not support optional beginning and
/// ending positions for the comparision. Use `str.substr()` as needed instead.
///
/// @param str string to test
/// @param prefix string to match
/// @return true if the string begins with the prefix
bool startswith(const std::string& str, const std::string& prefix);


/// Determine if a string ends with a suffix.
///
/// Unlike the Python version, this does not support optional beginning and
/// ending positions for the comparision. Use `str.substr()` as needed instead.
///
/// @param str string to test
/// @param suffix string to match
/// @return true if the string ends with the suffix
bool endswith(const std::string& str, const std::string& suffix);


/// Replace all occurrences of text in a string.
///
/// @param str string to modify
/// @param old text to replace
/// @param sub substitution text
/// @param maxcount maximum number of substitutions
/// @return modified string
std::string replace(std::string str, const std::string& old, const std::string& sub, ssize_t maxcount=-1);

}}  // namespace

#endif  // PYPP_STRING_HPP 
