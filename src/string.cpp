/// Implementation of the string module.
///
#include <cmath>
#include <locale>
#include <algorithm>
#include <iterator>
#include <ios>
#include <stdexcept>
#include "pypp/func.hpp"
#include "pypp/string.hpp"


using std::ceil;
using std::distance;
using std::find;
using std::floor;
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


char str::lower(char c) {
    return tolower(c, locale());
}


string str::lower(string str) {
    static const auto lambda([](char c){ return lower(c); });
    transform(str.begin(), str.end(), str.begin(), lambda);
    return str;
}


char str::upper(char c) {
    return toupper(c, locale());
}


string str::upper(string str) {
    static const auto lambda([](char c){ return upper(c); });
    transform(str.begin(), str.end(), str.begin(), lambda);
    return str;
}


string str::lstrip(const string& str, const string& chars) {
    const auto pos(str.find_first_not_of(chars));
    return pos == string::npos ? "" : str.substr(pos);
}


string str::rstrip(const string& str, const string& chars) {
    const auto pos(str.find_last_not_of(chars));
    return pos == string::npos ? "" : str.substr(0, pos + 1);
}


string str::strip(const string& str, const string& chars) {
    return rstrip(lstrip(str, chars), chars);
}


string str::join(const vector<string>& items, const string& sep) {
    // To conform with the Python join behavior, there is no special handling
    // of items that contain the separator, e.g. joining "abc" and "d,ef," will
    // produce "abc,d,ef,". This means that join() and split() are not strict
    // inverses of each other unless a distinct separator is used.
    string joined(items.front());
    for (auto iter(next(items.cbegin())); iter != items.cend(); ++iter) {
        // Skip first element to avoid leading delimiter.
        joined += sep + *iter;
    }
    return joined;
}


string str::join(const vector<string>& items, char sep) {
    return join(items, string(1, sep));
}


vector<string> str::split(const string& str, ssize_t maxsplit) {
    vector<string> items;
    auto beg(str.find_first_not_of(whitespace));
    while (beg <= str.size()) {
        auto end(str.size());
        if (maxsplit < 0 or items.size() < static_cast<size_t>(maxsplit)) {
            // Continue splitting.
            end = str.find_first_of(whitespace, beg);
            if (end == string::npos) {
                end = str.size();
            }
        }
        items.emplace_back(str.substr(beg, end - beg));
        beg = str.find_first_not_of(whitespace, end);
    }
    return items;
}


vector<string> str::split(const string& str, const string& sep, ssize_t maxsplit) {
    if (sep.empty()) {
        throw invalid_argument("empty separator");
    }
    vector<string> items;
    string::size_type beg(0);
    while (beg <= str.length()) {
        string::size_type end(str.size());
        if (maxsplit < 0 or items.size() < static_cast<size_t>(maxsplit)) {
            // Continue splitting.
            end = str.find(sep, beg);
            if (end == string::npos) {
                end = str.length();
            }
        }
        items.emplace_back(str.substr(beg, end - beg));
        beg = end + sep.length();
    }
    return items;
}


vector<string> str::rsplit(const string& str, ssize_t maxsplit) {
    const auto rstrip_len = [str](string::size_type end) {
        // Calculate the length of the string that would be returned by
        // rstrip(str.substr(len)).
        const auto pos(end != string::npos ? end - 1 : string::npos);
        const auto last(str.find_last_not_of(whitespace, pos));
        return last == string::npos ? 0 : last + 1;
    };
    vector<string> items;
    auto end(rstrip_len(string::npos));
    while (end > 0) {
        // Split the string from right to left such that each new item becomes
        // the first item in the sequence.
        const auto split(maxsplit < 0 or items.size() < static_cast<size_t>(maxsplit));
        if (not split) {
            // Consume the remainder of the string as the first item.
            items.insert(items.begin(), str.substr(0, end));
            break;
        }
        const auto pos(str.find_last_of(whitespace, end - 1));
        const auto beg(pos != string::npos ? pos + 1 : 0);
        items.insert(items.begin(), str.substr(beg, end - beg));
        end = rstrip_len(beg);
    }
    return items;
}


vector<string> str::rsplit(const string& str, const string& sep, ssize_t maxsplit) {
    if (sep.empty()) {
        throw invalid_argument("empty separator");
    }
    vector<string> items;
    auto end(str.size());
    while (true) {
        // Split the string from right to left such that each new item becomes
        // the first item in the sequence.
        const auto split(maxsplit < 0 or items.size() < static_cast<size_t>(maxsplit));
        const auto pos(str.rfind(sep, end - sep.size()));
        if (not split or end == 0 or pos == string::npos) {
            // Consume the remainder of the string as the first item.
            items.insert(items.begin(), str.substr(0, end));
            break;
        }
        const auto beg(pos + sep.size());
        items.insert(items.begin(), str.substr(beg, end - beg));
        end = pos;
    }
    return items;
}


bool str::startswith(const string& str, const string& prefix) {
    // Making a deliberate decision to break with Python functionality, which
    // supports optional beginning and ending positions for the comparison.
    // This can be easily duplicated using `str.substr()`, which avoids
    // burdening this function with what seems like an atypical use case.
    return str.substr(0, prefix.length()) == prefix;
}


bool str::startswith(const string& str, char prefix) {
    return str.front() == prefix;
}


bool str::endswith(const string& str, const string& suffix) {
    // Making a deliberate decision to break with Python functionality, which
    // supports optional beginning and ending positions for the comparison.
    // This can be easily duplicated using `str.substr()`, which avoids
    // burdening this function with what seems like an atypical use case.
    const auto strlen(str.length());
    const auto suflen(suffix.length());
    return suflen > strlen ? false : str.substr(strlen - suflen) == suffix;
}


string str::replace(string str, const string& old, const string& sub, ssize_t maxcount) {
    ssize_t count(0);
    string::size_type pos(0);
    if (not old.empty()) {
        while (maxcount == -1 or count++ < maxcount) {
            pos = str.find(old, pos);
            if (pos == string::npos) {
                break;
            }
            str.replace(pos, old.length(), sub);
            pos += sub.length();
        }
    }
    else {
        // An empty search string causes replace() to act like sub.join(str)
        // with leading and trailing delimiters.
        while (maxcount == -1 or count++ < maxcount) {
            str.insert(pos, sub);
            pos += sub.length() + 1;
            if (pos > str.length()) {
                break;
            }
        }
    }
    return str;
}


string str::center(const string& str, size_t width, char fill) {
    if (str.length() >= width) {
        return str;
    }
    const auto padlen(static_cast<double>(width - str.length()) / 2.);  // TODO: possible overflow
    const string lpad(static_cast<size_t>(floor(padlen)), fill);
    const string rpad(static_cast<size_t>(ceil(padlen)), fill);
    return lpad + str + rpad;
}
