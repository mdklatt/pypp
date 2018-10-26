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
#include <iterator>
#include <memory>
#include <deque>
#include <stdexcept>
#include "pypp/path.hpp"
#include "pypp/string.hpp"

using pypp::str::endswith;
using pypp::str::rstrip;
using pypp::str::startswith;
using std::deque;
using std::make_pair;
using std::pair;
using std::prev;
using std::runtime_error;
using std::string;
using std::unique_ptr;
using std::vector;

using namespace pypp;


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
    struct stat info;
    return stat(path.c_str(), &info) == 0;
}


bool path::isfile(const string& path)
{
    struct stat info;
    return stat(path.c_str(), &info) == 0 and S_ISREG(info.st_mode);
}


bool path::isdir(const string& path)
{
    struct stat info;
    return stat(path.c_str(), &info) == 0 and S_ISDIR(info.st_mode);
}
