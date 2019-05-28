/// Implementation of the path module.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "unistd.h"  // FIXME: not portable
#include "sys/stat.h"  // FIXME: not portable
#else
#error "path module requires *nix"
#endif

#include <cassert>
#include <cstdio>  // FILENAME_MAX
#include <algorithm>
#include <deque>
#include <iterator>
#include <memory>
#include <regex>
#include <stdexcept>
#include "pypp/path.hpp"
#include "pypp/string.hpp"

using pypp::str::endswith;
using pypp::str::rstrip;
using pypp::str::startswith;
using std::back_inserter;
using std::copy;
using std::deque;
using std::find;
using std::invalid_argument;
using std::make_pair;
using std::mismatch;
using std::pair;
using std::prev;
using std::regex;
using std::regex_match;
using std::runtime_error;
using std::string;
using std::tie;
using std::unique_ptr;
using std::vector;

using namespace pypp;
using path::PurePath;


const string path::sep("/");  // *nix, but works for Windows too


string path::join(const vector<string>& parts)
{
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


pair<string, string> path::split(const string& path)
{
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


string path::dirname(const string& path)
{
    return split(path).first;
}


string path::basename(const string& path)
{
    return split(path).second;
}


string path::normpath(const string& path)
{
    static const string current(".");
    static const string parent("..");
    ssize_t level(0);
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
            else if (not isabs(path)) {
                parts.push_back(parent);
            }
        }
        else {
            ++level;
            parts.push_back(item);
        }
    }
    string normed(join({parts.cbegin(), parts.cend()}));
    if (isabs(path)) {
        normed.insert(0, sep);
    }
    else if (normed.empty()) {
        normed = current;
    }
    return normed;
}


string path::abspath(const string& path)
{
    if (isabs(path)) {
        return normpath(path);
    }
    unique_ptr<char> cwd(new char[FILENAME_MAX]);
    if (not getcwd(cwd.get(), FILENAME_MAX)) {
        throw runtime_error("could not get current working directory");
    }
    return normpath(join({cwd.get(), path}));
}


bool path::isabs(const string& path)
{
    return startswith(path, sep);
}


pair<string, string> path::splitext(const string& path)
{
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


bool path::exists(const string& path)
{
    struct stat info{};
    return stat(path.c_str(), &info) == 0;
}


bool path::isfile(const string& path)
{
    struct stat info{};
    return stat(path.c_str(), &info) == 0 and S_ISREG(info.st_mode);
}


bool path::isdir(const string& path)
{
    struct stat info{};
    return stat(path.c_str(), &info) == 0 and S_ISDIR(info.st_mode);
}


PurePath::PurePath(string path)
{
    if (isabs(path)) {
        parts_.emplace_back(sep);
        path.erase(path.begin());
    }
    const auto parts(str::split(normpath(path), sep));
    if (parts.size() > 1 or parts.front() != ".") {
        copy(begin(parts), end(parts), back_inserter(parts_));
    }
}


PurePath PurePath::joinpath(const PurePath& other) const
{
    return PurePath(join({string(*this), string(other)}));
}


PurePath PurePath::joinpath(const std::string& other) const
{
    const vector<string> parts({string(*this), other});
    return PurePath(join(parts));
}


PurePath::operator std::string() const
{
    return parts_.empty() ? "." : str::join("", parts_);
}


const vector<string>& PurePath::parts() const
{
    return parts_;
}


PurePath PurePath::operator/(const string& other) const
{
    // Not sold on using division to join paths (why not addition?), but
    // sticking with the Python pathlib semantics for now.
    return joinpath(other);
}


PurePath PurePath::operator/(const PurePath& other) const
{
    return joinpath(other);
}


PurePath& PurePath::operator/=(const string& path)
{
    *this = joinpath(path);
    return *this;
}


PurePath& PurePath::operator/=(const PurePath& other)
{
    *this = joinpath(other);
    return *this;
}


bool PurePath::operator==(const PurePath& other) const
{
    return parts_ == other.parts_;
}


bool PurePath::operator!=(const PurePath& other) const
{
    return not (*this == other);
}


bool PurePath::is_absolute() const
{
    return parts_.empty() ? false : parts_.front() == sep;
}


string PurePath::name() const
{
    return is_root() ? "" : parts_.back();
}


PurePath PurePath::parent() const
{
    PurePath path(*this);
    const auto str{string(path)};
    if (not is_root()) {
        path.parts_.pop_back();
    }
    return path;
}


vector<PurePath> PurePath::parents() const
{
    vector<PurePath> paths;
    PurePath path(*this);
    while (not path.is_root()) {
        path = path.parent();
        paths.emplace_back(path);
    }
    return paths;
}


PurePath PurePath::relative_to(const PurePath& other) const
{
    const string error("path does not start with '" + string(other) + "'");
    if (parts_.size() < other.parts_.size()) {
        throw invalid_argument(error);
    }
    const auto diff(mismatch(other.parts_.begin(), other.parts_.end(), parts_.begin()));
    if (diff.first != other.parts_.end() and not other.parts_.empty()) {
        throw invalid_argument(error);
    }
    PurePath path;
    path.parts_ = {diff.second, parts_.end()};
    return path;
}


const std::string PurePath::root() const
{
    return is_absolute() ? "/" : "";
}


std::string PurePath::stem() const
{
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


string PurePath::suffix() const
{
    string suffix;
    tie(std::ignore, suffix) = splitext(name());
    if (suffix == ".") {
        // This is slightly different behavior than splitext(), which does not
        // ignore a solitary ".".
        suffix = "";
    }
    return suffix;
}


vector<string> PurePath::suffixes() const
{
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


PurePath PurePath::with_name(const string& name) const
{
    static const regex valid_name("^[^.][^" + sep + "]*$");
    if (not regex_match(name, valid_name)) {
        throw invalid_argument("invalid name '" + name + "'");
    }
    if (this->name().empty()) {
        throw invalid_argument("path has an empty name");
    }
    return parent() / name;
}


PurePath PurePath::with_suffix(const string& suffix) const
{
    static const regex valid_suffix("^\\.[^" + sep + "]+$");
    if (not (suffix.empty() or regex_match(suffix, valid_suffix))) {
        throw invalid_argument("invalid suffix '" + suffix + "'");
    }
    if (this->name().empty()) {
        throw invalid_argument("path has an empty name");
    }
    const auto name(stem() + suffix);
    return parent() / name;
}


bool PurePath::is_root() const
{
    return parts_.empty() or (parts_.size() == 1 and is_absolute());
}
