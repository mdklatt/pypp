/// Implemenation of the path module.
///
#include <iterator>
#include "path.hpp"
#include "string.hpp"

using pypp::endswith;
using pypp::startswith;
using std::make_pair;
using std::pair;
using std::prev;
using std::string;
using std::vector;

using namespace pypp;


const string path::sep("/");  // *nix, but works for Windows too


string path::join(const vector<string>& paths)
{
    // There is a discrepancy between the Python documentation and the actual
    // behavior of os.path.join(). From the documentation:
    //
    //   The return value is the concatenation of path and any members of
    //   *paths with exactly one directory separator (os.sep) following each
    //   non-empty part except the last, meaning that the result will only end
    //   in a separator if the last part is empty.
    //
    // Based on this, the result of `os.path.join("/abc//", "def/")` should be
    // "/abc/def", but the actual result is "/abc//def/".
    //
    // Here, the observed behavior of os.path.join() is reproduced. If a path
    // segment already has a trailing separator another one will not be added,
    // but redundant separators in the input are ignored. In practice both the
    // documented behavior and the actual behavior produce an equivalent path.
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
        if (not endswith(*iter, sep) and iter != last) {
            joined += sep;
        }
    }
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
};


string path::dirname(const std::string& path)
{
    return split(path).first;
}


string path::basename(const std::string& path)
{
    return split(path).second;
}
