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
    string::size_type pos(0);
    while (isspace(str[pos++]));  // one past first valid char
    return str.substr(--pos);
}


string pypp::rstrip(const string& str)
{
    string::size_type pos(str.size());
    while (isspace(str[--pos]));  // last valid char
    return str.substr(0, ++pos);
}


string pypp::strip(const string& str)
{
    return rstrip(lstrip(str));
}
