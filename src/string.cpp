/// Implementation of the string module.
///
#include <cctype>
#include <algorithm>
#include "string.hpp"


using std::string;
using std::tolower;
using std::toupper;
using std::transform;


string pypp::lower(string str)
{
    transform(str.begin(), str.end(), str.begin(), tolower);
    return str;
}


string pypp::upper(string str)
{
    transform(str.begin(), str.end(), str.begin(), toupper);
    return str;
}
