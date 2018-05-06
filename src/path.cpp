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
    // The Python documentation for os.path.join() is ambiguous about the
    // handling of path separators. Path separators are added as needed
    // between segments, while existing separators are left unmodified.
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
