/**
 * POSIX implementation of the 'path' module.
 */
#include <cassert>
#include <cstdio>
#include <deque>
#include <tuple>
#include <utility>
#include "pypp/os.hpp"
#include "pypp/path.hpp"
#include "pypp/string.hpp"


using pypp::os::getcwd;
using pypp::str::endswith;
using pypp::str::rstrip;
using pypp::str::startswith;
using std::pair;
using std::string;
using std::vector;

using namespace pypp;
using pypp::path::PureBasePath;


string path::join(const vector<string>& parts) {
    // The Python documentation for os.path.join() is ambiguous about the
    // handling of path separators. Path separators are added as needed
    // between segments, while existing separators are left unmodified.
    string joined;
    const auto last(prev(parts.cend()));
    for (auto iter(parts.cbegin()); iter != parts.cend(); ++iter) {
        if (startswith(*iter, sep)) {
            // Absolute path, ignore previous segments.
            joined = *iter;
        }
        else {
            joined += *iter;
        }
        if (not endswith(joined, sep) and iter != last) {
            joined += sep;
        }
    }
    assert(joined.length() <= FILENAME_MAX);
    return joined;
}


pair<string, string> path::split(const string& path) {
    auto pos(path.rfind(sep));
    if (pos == string::npos) {
        // No directory.
        return make_pair(string(""), path);
    }
    pos += sep.length();
    string root(path.substr(0, pos));
    if (root.find_first_not_of(sep) != string::npos) {
        // Strip trailing separators unless this is the root directory.
        root = rstrip(root, sep);
    }
    const auto name(path.substr(pos));
    return make_pair(root, name);
}


string path::dirname(const string& path) {
    return split(path).first;
}


string path::basename(const string& path) {
    return split(path).second;
}


string path::abspath(const string& path)
{
    if (isabs(path)) {
        return normpath(path);
    }
    return normpath(join({getcwd(), path}));
}


bool path::isabs(const string& path) {
    return startswith(path, sep);
}


pair<string, string> path::splitext(const string& path) {
    const auto pos(path.rfind('.'));
    string root;
    string ext;
    if (pos == 0 or pos == string::npos) {
        root = path;
    }
    else {
        root = path.substr(0, pos);
        ext = path.substr(pos, string::npos);
    }
    return make_pair(root, ext);
}


PureBasePath::PureBasePath(string path) {
    if (isabs(path)) {
        parts_.emplace_back(sep);
        path.erase(path.begin());
    }
    const auto parts(str::split(normpath(path), sep));
    if (parts.size() > 1 or parts.front() != ".") {
        copy(begin(parts), end(parts), back_inserter(parts_));
    }
}


PureBasePath::operator std::string() const {
    auto result(parts_.empty() ? "." : str::join(parts_, sep));
    if (is_absolute() and parts_.size() > 1) {
        // Fix the double slash at the beginning.
        result.erase(result.begin());
    }
    return result;
}


bool PureBasePath::is_absolute() const {
    return parts_.empty() ? false : parts_.front() == sep;
}


string PureBasePath::name() const {
    return is_root() ? "" : parts_.back();
}


string PureBasePath::root() const {
    return is_absolute() ? sep : "";
}


bool PureBasePath::is_root() const {
    return parts_.empty() or (parts_.size() == 1 and is_absolute());
}


string PureBasePath::stem() const {
    string stem;
    tie(stem, std::ignore) = splitext(name());
    if (stem == ".") {
        // This is slightly different behavior than splitext(), which does not
        // ignore a solitary ".".
        stem = "";
    }
    else if (not parts_.empty() and endswith(parts_.back(), ".")) {
        stem += ".";
    }
    return stem;
}


string PureBasePath::suffix() const {
    string suffix;
    tie(std::ignore, suffix) = splitext(name());
    if (suffix == ".") {
        // This is slightly different behavior than splitext(), which does not
        // ignore a solitary ".".
        suffix = "";
    }
    return suffix;
}


vector<string> PureBasePath::suffixes() const {
    const auto name(this->name());
    if (startswith(name, ".") or endswith(name, ".")) {
        return vector<string>();
    }
    auto suffixes = str::split(name, ".");
    auto pos(suffixes.erase(suffixes.begin()));  // ignore stem
    for (; pos != suffixes.end(); ++pos) {
        *pos = "." + *pos;
    }
    return suffixes;
}


short PureBasePath::compare(const PureBasePath& other) const {
    const string lhs(*this);
    const string rhs(other);
    if (lhs == rhs) {
        return 0;
    }
    return lhs < rhs ? -1 : 1;
}
