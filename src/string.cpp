/// Implementation of the string module.
///
#include <cctype>
#include <algorithm>
#include <iterator>
#include <ios>
#include <sstream>
#include <stdexcept>
#include "string.hpp"


using std::invalid_argument;
using std::max;
using std::next;
using std::string;
using std::transform;
using std::vector;


// TODO: Move out of the root namespace.
// The obvious choice for a module namespace is pypp::string, but that clashes
// with std::string and forces the use of fully qualified names everywhere.


const string pypp::whitespace(" \t\n\v\f\r");  // "C" locale


string pypp::lower(string str)
{
    // TODO: Use std::tolower for locale awareness.
    transform(str.begin(), str.end(), str.begin(), tolower);
    return str;
}


string pypp::upper(string str)
{
    // TODO: Use std::toupper for locale awareness.
    transform(str.begin(), str.end(), str.begin(), toupper);
    return str;
}


string pypp::lstrip(const string& str, const string& chars)
{
    const auto pos(str.find_first_not_of(chars));
    return pos == string::npos ? "" : str.substr(pos);
}


string pypp::rstrip(const string& str, const string& chars)
{
    auto pos(str.find_last_not_of(chars));
    return pos == string::npos ? "" : str.substr(0, ++pos);
}


string pypp::strip(const string& str, const string& chars)
{
    return rstrip(lstrip(str, chars), chars);
}


string pypp::join(const string& sep, const vector<string>& items)
{
    // If this was a clean-sheet design it would make more sense to put the
    // 'sep' argument last, and possibly give it a default value of "". The
    // goal here, however, is to resemble Python, where this function is used
    // as a member function of the separator string.
    //
    // To conform with the Python join behavior, there is no special handling
    // of items that contain the separator, e.g. joining "abc" and "d,ef," will
    // produce "abc,d,ef,". This means that that join() and split() are not
    // strict inverses of each other unless the caller takes care to pick a
    // separator that is distinct from any input.
    string joined(items.front());
    for (auto iter(next(items.cbegin())); iter != items.cend(); ++iter) {
        // Skip first element to avoid leading delimiter.
        joined += sep + *iter;
    }
    return joined;
}


vector<string> pypp::split(const string& str, ssize_t maxsplit)
{
    vector<string> items;
    std::istringstream stream(str);
    ssize_t count(0);
    string item;
    while (stream >> item) {
        if (maxsplit >= 0 and count++ >= maxsplit) {
            // Exceeded max splits, consume the rest of the string.
            char c;
            while (stream.get(c)) {
                // TODO: This seems very inefficient.
                item += c;
            }
        }
        items.emplace_back(item);
    }
    return items;
}


vector<string> pypp::split(const string& str, const string& sep, ssize_t maxsplit)
{
    if (sep.empty()) {
        throw invalid_argument("empty separator");
    }
    const auto len(str.length());
    vector<string> items;
    string::size_type beg(0);
    ssize_t count(0);
    while (beg <= len) {
        string::size_type pos(len);
        if (maxsplit < 0 or count++ < maxsplit) {
            // Continue splitting.
            pos = str.find(sep, beg);
            if (pos == string::npos) {
                pos = str.length();
            }
        }
        items.emplace_back(str.substr(beg, pos - beg));
        beg = pos + sep.length();
    }
    return items;
}


bool pypp::startswith(const string& str, const string& prefix)
{
    // Making a deliberate decision to break with Python functionality, which
    // supports optional beginning and ending positions for the comparison.
    // This can be easily duplicated using `str.substr()`, which avoids
    // burdening this function with what seems like an atypical use case.
    return str.substr(0, prefix.length()) == prefix;
}


bool pypp::endswith(const string& str, const string& suffix)
{
    // Making a deliberate decision to break with Python functionality, which
    // supports optional beginning and ending positions for the comparison.
    // This can be easily duplicated using `str.substr()`, which avoids
    // burdening this function with what seems like an atypical use case.
    const auto strlen(str.length());
    const auto suflen(suffix.length());
    return suflen > strlen ? false : str.substr(strlen - suflen) == suffix;
}
