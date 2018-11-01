/// Implementation of the string module.
///
#include <locale>
#include <algorithm>
#include <iterator>
#include <ios>
#include <sstream>
#include <stdexcept>
#include "pypp/string.hpp"


using std::invalid_argument;
using std::locale;
using std::max;
using std::next;
using std::string;
using std::tolower;
using std::transform;
using std::vector;

using namespace pypp;


const string str::whitespace(" \t\n\v\f\r");  // "C" locale


char str::lower(char c)
{
    return tolower(c, locale());
}


string str::lower(string str)
{
    static const auto lambda([](char c){ return lower(c); });
    transform(str.begin(), str.end(), str.begin(), lambda);
    return str;
}


char str::upper(char c)
{
    return toupper(c, locale());
}


string str::upper(string str)
{
    static const auto lambda([](char c){ return upper(c); });
    transform(str.begin(), str.end(), str.begin(), lambda);
    return str;
}


string str::lstrip(const string& str, const string& chars)
{
    const auto pos(str.find_first_not_of(chars));
    return pos == string::npos ? "" : str.substr(pos);
}


string str::rstrip(const string& str, const string& chars)
{
    auto pos(str.find_last_not_of(chars));
    return pos == string::npos ? "" : str.substr(0, ++pos);
}


string str::strip(const string& str, const string& chars)
{
    return rstrip(lstrip(str, chars), chars);
}


string str::join(const string& sep, const vector<string>& items)
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


string str::join(char sep, const vector<string>& items)
{
    return join(string(1, sep), items);
}


vector<string> str::split(const string& str, ssize_t maxsplit)
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


vector<string> str::split(const string& str, const string& sep, ssize_t maxsplit)
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


bool str::startswith(const string& str, const string& prefix)
{
    // Making a deliberate decision to break with Python functionality, which
    // supports optional beginning and ending positions for the comparison.
    // This can be easily duplicated using `str.substr()`, which avoids
    // burdening this function with what seems like an atypical use case.
    return str.substr(0, prefix.length()) == prefix;
}


bool str::endswith(const string& str, const string& suffix)
{
    // Making a deliberate decision to break with Python functionality, which
    // supports optional beginning and ending positions for the comparison.
    // This can be easily duplicated using `str.substr()`, which avoids
    // burdening this function with what seems like an atypical use case.
    const auto strlen(str.length());
    const auto suflen(suffix.length());
    return suflen > strlen ? false : str.substr(strlen - suflen) == suffix;
}


string str::replace(string str, const string& old, const string& sub, ssize_t maxcount)
{
    ssize_t count(0);
    string::size_type pos(0);
    while (maxcount == -1 or count < maxcount) {
        // TODO: Split this into two loops to avoid repeated ifs?
        if (not old.empty()) {
            pos = str.find(old, pos);
            if (pos == string::npos) {
                break;
            }
            str.replace(pos, old.length(), sub);
            pos += sub.length();
        }
        else {
            // In Python, an empty search string causes replace() to act like
            // sub.join(str) with leading and trailing delimiters. Not sure
            // what the use case for that is, but it's reproduced here.
            str.insert(pos, sub);
            pos += sub.length() + 1;
            if (pos > str.length()) {
                break;
            }
        }
        ++count;
    }
    return str;
}
