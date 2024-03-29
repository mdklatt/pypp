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


/// Convert a character to lower case.
///
/// @param c input char
/// @return lower-case char
char lower(char c);


/// Convert a string to lower case.
///
/// @param str input string
/// @return lower-case string
std::string lower(std::string str);


/// Convert a char to upper case.
///
/// @param c input char
/// @return upper-case char
char upper(char c);


/// Convert a string to upper case.
///
/// @param str input string
/// @return upper-case string
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


/**
 * Join strings using a separator.
 *
 * @param items: strings to join
 * @param sep: separator
 * @return: joined values
 */
// TODO: take iterators?
std::string join(const std::vector<std::string>& items, const std::string& sep="");


/** @overload */
std::string join(const std::vector<std::string>& items, char sep);


/// Split a string on whitespace.
///
/// If `maxsplit` is non-negative, a maximum of that many splits will be
/// performed, and the last item will contain the remainder of the string
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
/// performed, and the last item will contain the remainder of the string
/// regardless of any separators it contains.
///
/// @param str string to split
/// @param sep separator to split on
/// @param maxsplit maximum number of splits to perform
/// @return split items
std::vector<std::string> split(const std::string& str, const std::string& sep, ssize_t maxsplit=-1);


/// Split a string on whitespace starting from the right.
///
/// If `maxsplit` is non-negative, a maximum of that many splits will be
/// performed, and the first item will contain the remainder of the string
/// regardless of any whitespace it contains.
///
/// Leading and trailing whitespace is ignored.
///
/// @param str string to split
/// @param maxsplit maximum number of splits to perform
/// @return split items
std::vector<std::string> rsplit(const std::string& str, ssize_t maxsplit=-1);


/// Split a string on a separator starting from the right.
///
/// All occurrences of the separator are significant, and will generate empty
/// strings as items as appropriate.
///
/// If `maxsplit` is non-negative, a maximum of that many splits will be
/// performed, and the first item will contain the remainder of the string
/// regardless of any separators it contains.
///
/// @param str string to split
/// @param sep separator to split on
/// @param maxsplit maximum number of splits to perform
/// @return split items
std::vector<std::string> rsplit(const std::string& str, const std::string& sep, ssize_t maxsplit=-1);


/**
 * Determine if a string starts with a prefix.
 *
 * Unlike the Python version, this does not support optional beginning and
 * ending positions for the comparision. Use `str.substr()` as needed instead.
 *
 * @param str: string to test
 * @param prefix: prefix to match
 * @return: true if the string begins with the prefix
 */
bool startswith(const std::string& str, const std::string& prefix);


/** @overload */
bool startswith(const std::string& str, char prefix);


/**
 * Determine if a string ends with a suffix.
 *
 * Unlike the Python version, this does not support optional beginning and
 * ending positions for the comparision. Use `str.substr()` as needed instead.
 *
 * @param str: string to test
 * @param suffix: suffix to match
 * @return: true if the string ends with the suffix
 */
bool endswith(const std::string& str, const std::string& suffix);


/** @overload */
bool endswith(const std::string& str, char prefix);


/// Replace all occurrences of text in a string.
///
/// @param str input string
/// @param old text to replace
/// @param sub substitution text
/// @param maxcount maximum number of substitutions
/// @return modified string
std::string replace(std::string str, const std::string& old, const std::string& sub, ssize_t maxcount=-1);


/// Pad both sides of string to center it.
///
/// If the amount of padding is not even, the extra fill character will be on
/// the right.
///
/// @param str input string
/// @param width total output width
/// @param fill fill character
/// @return padded string
std::string center(const std::string& str, size_t width, char fill=' ');

}}  // namespace

#endif  // PYPP_STRING_HPP 
