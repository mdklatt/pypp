/// Implementation of the path module.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "unistd.h"
#include "sys/stat.h"
#else
#error "path module requires *nix"
#endif

#include <cassert>
#include <cstdio>  // FILENAME_MAX
#include <algorithm>
#include <deque>
#include <iterator>
#include <map>
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
using std::fstream;
using std::ios_base;
using std::invalid_argument;
using std::make_pair;
using std::map;
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
using path::PurePosixPath;
using path::PosixPath;


// In the Python implementation, a Path is derived from a PurePath, and
// metaprogramming is used to ensure that methods return an object of the
// correct type. All of the C++ solutions for this will have a negative impact
// on usability. Instead, the strategy here is to use private inheritance to
// ensure that a Path acts just like a PurePath. This means that a Path is not
// recognizable as a PurePath, but the use case for interoperability seems
// limited, and can be handled by adding explicit conversion methods.


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
    unique_ptr<char> buffer(new char[FILENAME_MAX]);
    if (not getcwd(buffer.get(), FILENAME_MAX)) {
        throw runtime_error("could not get current working directory");
    }
    return normpath(join({buffer.get(), path}));
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


PurePosixPath::PurePosixPath(string path)
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


PurePosixPath PurePosixPath::joinpath(const PurePosixPath& other) const
{
    return PurePosixPath(join({string(*this), string(other)}));
}


PurePosixPath PurePosixPath::joinpath(const std::string& other) const
{
    const vector<string> parts({string(*this), other});
    return PurePosixPath(join(parts));
}


PurePosixPath::operator std::string() const
{
    auto result(parts_.empty() ? "." : str::join(sep, parts_));
    if (is_absolute() and parts_.size() > 1) {
        // Fix the double slash at the beginning.
        result.erase(result.begin());
    }
    return result;
}


const vector<string>& PurePosixPath::parts() const
{
    return parts_;
}


PurePosixPath PurePosixPath::operator/(const string& other) const
{
    // Not sold on using division to join paths (why not addition?), but
    // sticking with the Python pathlib semantics for now.
    return joinpath(other);
}


PurePosixPath PurePosixPath::operator/(const PurePosixPath& other) const
{
    return joinpath(other);
}


PurePosixPath& PurePosixPath::operator/=(const string& path)
{
    *this = joinpath(path);
    return *this;
}


PurePosixPath& PurePosixPath::operator/=(const PurePosixPath& other)
{
    *this = joinpath(other);
    return *this;
}


bool PurePosixPath::operator==(const PurePosixPath& other) const
{
    return parts_ == other.parts_;
}


bool PurePosixPath::operator!=(const PurePosixPath& other) const
{
    return not (*this == other);
}


bool PurePosixPath::is_absolute() const
{
    return parts_.empty() ? false : parts_.front() == sep;
}


string PurePosixPath::name() const
{
    return is_root() ? "" : parts_.back();
}


PurePosixPath PurePosixPath::parent() const
{
    PurePosixPath path(*this);
    const auto str{string(path)};
    if (not is_root()) {
        path.parts_.pop_back();
    }
    return path;
}


vector<PurePosixPath> PurePosixPath::parents() const
{
    vector<PurePosixPath> paths;
    PurePosixPath path(*this);
    while (not path.is_root()) {
        path = path.parent();
        paths.emplace_back(path);
    }
    return paths;
}


PurePosixPath PurePosixPath::relative_to(const PurePosixPath& other) const
{
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


std::string PurePosixPath::root() const
{
    return is_absolute() ? "/" : "";
}


std::string PurePosixPath::stem() const
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


string PurePosixPath::suffix() const
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


vector<string> PurePosixPath::suffixes() const
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


PurePosixPath PurePosixPath::with_name(const string& name) const
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


PurePosixPath PurePosixPath::with_suffix(const string& suffix) const
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


bool PurePosixPath::is_root() const
{
    return parts_.empty() or (parts_.size() == 1 and is_absolute());
}


// Implement the PurePath portion of the Path API. Type-specific methods that
// cannot be pulled down from the base base via `using` are implemented here in
// terms of their PurePath implementation.

PosixPath::PosixPath(const std::string& path):
    PurePosixPath(path)
{}

PosixPath::PosixPath(const pypp::path::PurePosixPath& path):
    PurePosixPath(path)
{}


bool PosixPath::operator==(const PosixPath& other) const
{
    return PurePosixPath::operator==(other);
}


bool PosixPath::operator!=(const PosixPath& other) const
{
    return PurePosixPath::operator!=(other);
}


PosixPath PosixPath::joinpath(const PosixPath& path) const
{
    return PosixPath(PurePosixPath::joinpath(path));
}


PosixPath PosixPath::joinpath(const string& path) const
{
    return PosixPath(PurePosixPath::joinpath(path));
}


PosixPath PosixPath::operator/(const PosixPath& path) const
{
    return PosixPath(PurePosixPath::operator/(path));
}


PosixPath PosixPath::operator/(const string& path) const
{
    return PosixPath(PurePosixPath::operator/(path));
}


PosixPath& PosixPath::operator/=(const PosixPath& other)
{
    PurePosixPath::operator/=(other);
    return *this;
}


PosixPath& PosixPath::operator/=(const std::string& other)
{
    PurePosixPath::operator/=(other);
    return *this;
}


PosixPath PosixPath::parent() const
{
    return PosixPath(PurePosixPath::parent());
}


vector<PosixPath> PosixPath::parents() const
{
    vector<PosixPath> result;
    for (const auto& path: PurePosixPath::parents()) {
        // Convert each item to the correct type.
        result.emplace_back(PosixPath(path));
    }
    return result;
}

PosixPath PosixPath::relative_to(const PosixPath& other) const
{
    return PosixPath(PurePosixPath::relative_to(other));
}


PosixPath PosixPath::with_name(const std::string& name) const
{
    return PosixPath(PurePosixPath::with_name(name));
}

PosixPath PosixPath::with_suffix(const std::string& name) const
{
    return PosixPath(PurePosixPath::with_suffix(name));
}


// Implement the PosixPath-specific API.

PosixPath PosixPath::cwd()
{
    unique_ptr<char> buffer(new char[FILENAME_MAX]);
    if (not getcwd(buffer.get(), FILENAME_MAX)) {
        throw runtime_error("could not get current working directory");
    }
    return PosixPath(string(buffer.get()));
}


PurePosixPath PosixPath::pure() const
{
    // This is not part of the Python API, but is useful here because the
    // private inheritance relationship between PosixPath and PurePosixPath
    // otherwise prevents interoperability.
    return PurePosixPath(*this);
}


bool PosixPath::exists() const
{
    return path::exists(string(*this));
}


bool PosixPath::is_dir() const
{
    return path::isdir(string(*this));
}


bool PosixPath::is_file() const
{
    return path::isfile(string(*this));
}


fstream PosixPath::open(const string& mode) const
{
    // The Python implementation throws an exception if the file cannot be
    // opened. The only reliable way to test the validity of an open C++
    // stream, however, is to attempt a read or write operation on it; calling
    // is_open() and good() on the stream is *not* sufficient. The caller is
    // responsible for doing I/O on the returned stream in a fail-safe way.
    static const map<char, ios_base::openmode> modes({
        {'r', fstream::in},
        {'w', fstream::out|fstream::trunc},
        {'x', fstream::out|fstream::trunc},
        {'a', fstream::out|fstream::app},
    });
    const string path(*this);
    const auto it(modes.find(mode[0]));
    if (it == modes.end()) {
        // This not a stream error so throw an exception.
        throw invalid_argument("invalid file mode: '" + mode + "'");
    }
    if (it->first == 'x' and exists()) {
        // As of C++11, there's no way to do this via iostream flags. For
        // consistency with the other modes, treat this like an iostream error
        // and return an invalid stream instead of throwing an exception.
        return fstream();
    }
    auto flags(it->second);
    if (mode.size() >= 2 and mode[1] == '+') {
        // Make the stream readable and writable, or vice versa.
        flags |= fstream::in | fstream::out;
    }
    if (endswith(mode, "b")) {
        flags |= fstream::binary;
    }
    return fstream(path, flags);
}
