/// Implementation of the string module.
///
#include <cctype>
#include <algorithm>
#include <iterator>
#include "string.hpp"


using std::next;
using std::string;
using std::transform;
using std::vector;


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


string pypp::join(const vector<string>& items, const string& delim)
{
    string joined(items.front());
    for (auto iter(next(items.cbegin())); iter != items.cend(); ++iter) {
        // Skip first element to avoid leading delimiter.
        joined += delim + *iter;
    }
    return joined;
}
