/// Implementation of the string module.
///
#include <cctype>
#include <algorithm>
#include "string.hpp"


using std::string;
using std::transform;


string pypp::lower(string str)
{
    // TODO: Use std::tolower for locale awareness.
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}


string pypp::upper(string str)
{
    // TODO: Use std::toupper for locale awareness.
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}


string pypp::lstrip(const string& str)
{
    const auto end(str.cend());
    auto pos(str.cbegin());
    for (; pos != end; ++pos) {
        if (not isspace(*pos)) {
            break;  // at first valid character
        }
    }
    return string(pos, end);
}


string pypp::rstrip(const string& str)
{
    auto pos(str.crbegin());
    for (; pos != str.crend(); ++pos) {
        if (not isspace(*pos)) {
            break;  // at last valid character
        }
    }
    return string(str.cbegin(), pos.base());
}


string pypp::strip(const string& str)
{
    return rstrip(lstrip(str));
}
