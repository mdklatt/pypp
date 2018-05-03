/// Implementation of the string module.
///
#include <cctype>
#include <algorithm>
#include <iterator>
#include <ios>
#include <sstream>
#include "string.hpp"


using std::next;
using std::string;
using std::transform;
using std::vector;


namespace {  // internal linkage

/// Split string on whitespace.
///
vector<string> split_space(const string& str, ssize_t maxsplit)
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


/// Split string on a delimiter.
///
vector<string> split_delim(const string& str, const string& sep, ssize_t maxsplit)
{
    vector<string> items;
    string::size_type beg(0);
    ssize_t count(0);
    while (beg <= str.length()) {
        string::size_type pos(0);
        if (maxsplit >= 0 and count++ >= maxsplit) {
            // Exceeded max splits, consume the rest of the string.
            pos = str.length();
        }
        else {
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

}  // namespace


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


string pypp::lstrip(const string& str)
{
    // TODO: Use std::isspace for locale awareness.
    const auto end(str.cend());
    auto iter(str.cbegin());
    for (; iter != end; ++iter) {
        if (not isspace(*iter)) {
            // At first valid character.
            break;
        }
    }
    return string(iter, end);
}


string pypp::rstrip(const string& str)
{
    // TODO: Use std::isspace for locale awareness.
    auto iter(str.crbegin());
    for (; iter != str.crend(); ++iter) {
        if (not isspace(*iter)) {
            // At last valid character.
            break;
        }
    }
    return string(str.cbegin(), iter.base());
}


string pypp::strip(const string& str)
{
    return rstrip(lstrip(str));
}


string pypp::join(const vector<string>& items, const string& sep)
{
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


vector<string> pypp::split(const string& str, const string& sep, ssize_t maxsplit)
{
    return sep.empty() ? split_space(str, maxsplit) : split_delim(str, sep, maxsplit);
}

