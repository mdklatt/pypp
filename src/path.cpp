/**
 * System-independent components of the 'path' module.
 */
#include <cassert>
#include <cstdio>
#include <deque>
#include <regex>
#include <stdexcept>
#include <tuple>
#include <utility>
#include "pypp/os.hpp"
#include "pypp/path.hpp"
#include "pypp/string.hpp"


using pypp::os::getcwd;
using pypp::str::endswith;
using pypp::str::rstrip;
using pypp::str::startswith;
using std::deque;
using std::invalid_argument;
using std::pair;
using std::regex;
using std::regex_match;
using std::string;
using std::vector;

using namespace pypp;
using path::PureBasePath;
using path::PurePosixPath;
using path::PureWindowsPath;


namespace {

// Platform-agnostic implementations that are not part of the public API.

/**
 * Determine if a path is absolute.
 *
 * @param path: input path
 * @param sep: path separator
 * @return: true for an absolute path
 */
bool isabs(const string& path, const string& sep) {
    return startswith(path, sep);
}


/**
 * Join paths into a single path.
 *
 * Use an empty string as the last to segment to ensure that the path ends in a
 * trailing separator.
 *
 * @param paths: paths to join
 * @param sep: path separator
 * @return: joined path
 */
string join(const vector<string>& paths, const string& sep) {
    // The Python documentation for os.path.join() is ambiguous about the
    // handling of path separators, stating that the joined path has "exactly
    // one directory separator following each non-empty part". However, the
    // result of `os.path.join("abc//", "xyz")` is `abc//xzy`. That is, the
    // extra separator in "abc//" is left as-is. That behavior is reproduced
    // here.
    string joined;
    const auto last(prev(paths.cend()));
    for (auto iter(paths.cbegin()); iter != paths.cend(); ++iter) {
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
    assert(joined.length() <= FILENAME_MAX);  // TODO: exception?
    return joined;
}


/**
 * Split a path into directory and name components.
 *
 * If the path has a trailing separator, the pathname component will be empty.
 * Calling join() on the resulting segments will return an equivalent (but not
 * necessarily identical) path.
 *
 * @param path: input path
 * @param sep: path separator
 * @return: (root, name) pair
 */
pair<string, string> split(const string& path, const string& sep) {
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


/**
 * Normalize a path.
 *
 * @param path: input path
 * @param sep: path separator
 * @return normalized path
 */
string normpath(const std::string& path, const std::string& sep) {
    static const string current{"."};
    static const string parent{".."};
    ssize_t level{0};
    deque<string> parts;
    for (const auto& item: str::split(path, sep)) {
        // Process each part of the path.
        if (item.empty() or item == current) {
            continue;
        }
        if (item == parent) {
            --level;
            if (level >= 0) {
                parts.pop_back();
            }
            else if (not path::isabs(path)) {
                parts.push_back(parent);
            }
        }
        else {
            ++level;
            parts.push_back(item);
        }
    }
    string normed(join({parts.cbegin(), parts.cend()}, sep));
    if (isabs(path, sep)) {
        normed.insert(0, sep);
    }
    else if (normed.empty()) {
        normed = current;
    }
    return normed;
}

}  // internal linkage


string path::join(const vector<string>& paths) {
    return ::join(paths, SEP);
}


pair<string, string> path::split(const string& path) {
    return ::split(path, SEP);
}


string path::dirname(const string& path) {
    return split(path).first;
}


string path::basename(const string& path) {
    return split(path).second;
}


string path::normpath(const std::string& path) {
    return ::normpath(path, SEP);
}


string path::abspath(const string& path) {
    if (isabs(path)) {
        return normpath(path);
    }
    return normpath(join({getcwd(), path}));
}


bool path::isabs(const string& path) {
    return ::isabs(path, SEP);
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


PureBasePath::PureBasePath(string path, const string& sep) {
    if (::isabs(path, sep)) {
        parts_.emplace_back(sep);
        path.erase(path.begin());
    }
    const auto parts(str::split(::normpath(path, sep), sep));
    if (parts.size() > 1 or parts.front() != ".") {
        copy(begin(parts), end(parts), back_inserter(parts_));
    }
}


PureBasePath::operator std::string() const {
    auto result(parts_.empty() ? "." : str::join(parts_, sep()));
    if (is_absolute() and parts_.size() > 1) {
        // Fix the double slash at the beginning.
        result.erase(result.begin());
    }
    return result;
}


bool PureBasePath::is_absolute() const {
    return not parts_.empty() and parts_.front() == sep();
}


string PureBasePath::name() const {
    return is_root() ? "" : parts_.back();
}


string PureBasePath::root() const {
    return is_absolute() ? sep() : "";
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
        return {};
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


void PureBasePath::set_name(const std::string& name) {
    // The backslash must be escaped inside a regular expression, so for
    // Windows the path separator needs to be escaped twice, i.e. "\\\\"
    static const string esc("\\");
    const auto sep(this->sep());
    const auto sep_re(sep == esc ? esc + sep : sep);
    const regex valid_name("^[^.][^" + sep_re + "]*$");
    if (not regex_match(name, valid_name)) {
        throw invalid_argument("invalid name '" + name + "'");
    }
    if (this->name().empty()) {
        throw invalid_argument("path has an empty name");
    }
    parts_.back() = name;
    return;
}


void PureBasePath::set_suffix(const string& suffix) {
    // The backslash must be escaped inside a regular expression, so for
    // Windows the path separator needs to be escaped twice, i.e. "\\\\"
    static const string esc("\\");
    const auto sep(this->sep());
    const auto sep_re(sep == esc ? esc + sep : sep);
    const regex valid_suffix("^\\.[^" + sep_re + "]+$");
    if (not (suffix.empty() or regex_match(suffix, valid_suffix))) {
        throw invalid_argument("invalid suffix '" + suffix + "'");
    }
    if (this->name().empty()) {
        throw invalid_argument("path has an empty name");
    }
    set_name(stem() + suffix);
    return;
}


PurePosixPath::PurePosixPath(string path): PureBasePath(move(path), "/") {}


PurePosixPath PurePosixPath::joinpath(const PurePosixPath& other) const {
    return joinpath(string(other));
}


PurePosixPath PurePosixPath::joinpath(const string& other) const {
    const vector<string> parts({string(*this), other});
    return PurePosixPath(::join(parts, sep()));
}


PurePosixPath PurePosixPath::operator/(const string& other) const {
    // Not sold on using division to join paths (why not addition?), but
    // sticking with the Python pathlib semantics for now.
    return joinpath(other);
}


PurePosixPath PurePosixPath::operator/(const PurePosixPath& other) const {
    return joinpath(other);
}


PurePosixPath& PurePosixPath::operator/=(const string& path) {
    *this = joinpath(path);
    return *this;
}


PurePosixPath& PurePosixPath::operator/=(const PurePosixPath& other) {
    *this = joinpath(other);
    return *this;
}


bool PurePosixPath::operator==(const PurePosixPath& other) const {
    return compare(other) == 0;
}


bool PurePosixPath::operator!=(const PurePosixPath& other) const {
    return not (*this == other);
}


bool PurePosixPath::operator<(const PurePosixPath& other) const {
    return compare(other) < 0;
}


PurePosixPath PurePosixPath::parent() const {
    PurePosixPath path(*this);
    const string str(path);
    if (not is_root()) {
        path.parts_.pop_back();
    }
    return path;
}


vector<PurePosixPath> PurePosixPath::parents() const {
    vector<PurePosixPath> paths;
    PurePosixPath path(*this);
    while (not path.is_root()) {
        path = path.parent();
        paths.emplace_back(path);
    }
    return paths;
}


PurePosixPath PurePosixPath::relative_to(const PurePosixPath& other) const {
    const string error("path does not start with '" + string(other) + "'");
    if (parts_.size() < other.parts_.size()) {
        throw invalid_argument(error);
    }
    const auto diff(mismatch(other.parts_.begin(), other.parts_.end(), parts_.begin()));
    if (diff.first != other.parts_.end() and not other.parts_.empty()) {
        throw invalid_argument(error);
    }
    PurePosixPath path;
    path.parts_ = {diff.second, parts_.end()};
    return path;
}


PurePosixPath PurePosixPath::with_name(const string& name) const {
    PurePosixPath path(*this);
    path.set_name(name);
    return path;
}


PurePosixPath PurePosixPath::with_suffix(const string& suffix) const {
    PurePosixPath path(*this);
    path.set_suffix(suffix);
    return path;
}


PureWindowsPath::PureWindowsPath(string path): PureBasePath(move(path), "\\") {}


PureWindowsPath PureWindowsPath::joinpath(const PureWindowsPath& other) const {
    return joinpath(string(other));
}


PureWindowsPath PureWindowsPath::joinpath(const string& other) const {
    return PureWindowsPath(::join({string(*this), other}, sep()));
}


PureWindowsPath PureWindowsPath::operator/(const string& other) const {
    // Not sold on using division to join paths (why not addition?), but
    // sticking with the Python pathlib semantics for now.
    return joinpath(other);
}


PureWindowsPath PureWindowsPath::operator/(const PureWindowsPath& other) const {
    return joinpath(other);
}


PureWindowsPath& PureWindowsPath::operator/=(const string& path) {
    *this = joinpath(path);
    return *this;
}


PureWindowsPath& PureWindowsPath::operator/=(const PureWindowsPath& other) {
    *this = joinpath(other);
    return *this;
}


bool PureWindowsPath::operator==(const PureWindowsPath& other) const {
    return compare(other) == 0;
}


bool PureWindowsPath::operator!=(const PureWindowsPath& other) const {
    return not (*this == other);
}


bool PureWindowsPath::operator<(const PureWindowsPath& other) const {
    return compare(other) < 0;
}


PureWindowsPath PureWindowsPath::parent() const {
    PureWindowsPath path(*this);
    const string str(path);
    if (not is_root()) {
        path.parts_.pop_back();
    }
    return path;
}


vector<PureWindowsPath> PureWindowsPath::parents() const {
    vector<PureWindowsPath> paths;
    PureWindowsPath path(*this);
    while (not path.is_root()) {
        path = path.parent();
        paths.emplace_back(path);
    }
    return paths;
}


PureWindowsPath PureWindowsPath::relative_to(const PureWindowsPath& other) const {
    const string error{"path does not start with '" + string(other) + "'"};
    if (parts_.size() < other.parts_.size()) {
        throw invalid_argument(error);
    }
    const auto diff(mismatch(other.parts_.begin(), other.parts_.end(), parts_.begin()));
    if (diff.first != other.parts_.end() and not other.parts_.empty()) {
        throw invalid_argument(error);
    }
    PureWindowsPath path;
    path.parts_ = {diff.second, parts_.end()};
    return path;
}


PureWindowsPath PureWindowsPath::with_name(const string& name) const {
    PureWindowsPath path(*this);
    path.set_name(name);
    return path;
}


PureWindowsPath PureWindowsPath::with_suffix(const string& suffix) const {
    PureWindowsPath path(*this);
    path.set_suffix(suffix);
    return path;
}
