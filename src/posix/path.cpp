/**
 * Implementations of the 'path' module.
 */
#include "unistd.h"
#include "sys/stat.h"
#include "dirent.h"
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <utility>
#include "pypp/func.hpp"
#include "pypp/os.hpp"
#include "pypp/path.hpp"
#include "pypp/string.hpp"

using pypp::func::in;
using pypp::os::getcwd;
using pypp::os::makedirs;
using pypp::str::endswith;
using pypp::str::rstrip;
using pypp::str::startswith;
using std::back_inserter;
using std::copy;
using std::find;
using std::fstream;
using std::ios_base;
using std::invalid_argument;
using std::make_pair;
using std::map;
using std::mismatch;
using std::move;
using std::ostringstream;
using std::pair;
using std::prev;
using std::remove;
using std::runtime_error;
using std::string;
using std::tie;
using std::unique_ptr;
using std::vector;

using namespace pypp;
using path::PurePosixPath;
using path::PosixPath;


const string path::SEP("/");


// In the Python implementation a Path inherits from a PurePath, and
// metaprogramming is used to ensure that methods return an object of the
// correct type. All of the C++ solutions for this will have a negative impact
// on usability. Instead, the strategy here is to use composition. This means
// that a Path is not recognizable as a PurePath, but the use case for
// interoperability seems limited, and can be handled using explicit conversion
// methods.


bool path::exists(const string& path) {
    struct stat info{};
    return stat(path.c_str(), &info) == 0;
}


bool path::isfile(const string& path) {
    struct stat info{};
    return stat(path.c_str(), &info) == 0 and S_ISREG(info.st_mode);
}


bool path::isdir(const string& path) {
    struct stat info{};
    return stat(path.c_str(), &info) == 0 and S_ISDIR(info.st_mode);
}


bool path::islink(const string& path) {
    struct stat info{};
    return lstat(path.c_str(), &info) == 0 and S_ISLNK(info.st_mode);
}


// Implement the PurePath portion of the Path API.

PosixPath::PosixPath(const std::string& path):
    base_(path)
{}


PosixPath::PosixPath(PurePosixPath path):
    base_(move(path))
{}


PosixPath::operator string() const
{
    return string(base_);
}


bool PosixPath::is_absolute() const
{
    return base_.is_absolute();
}


std::string PosixPath::name() const
{
    return base_.name();
}


const vector<string>& PosixPath::parts() const
{
    return base_.parts();
}


string PosixPath::root() const
{
    return base_.root();
}


string PosixPath::stem() const
{
    return base_.stem();
}


string PosixPath::suffix() const
{
    return base_.suffix();
}


vector<string> PosixPath::suffixes() const
{
    return base_.suffixes();
}


bool PosixPath::operator==(const PosixPath& other) const
{
    return base_ == other.base_;
}


bool PosixPath::operator!=(const PosixPath& other) const
{
    return base_ != other.base_;
}


bool PosixPath::operator<(const PosixPath& other) const
{
    return base_ < other.base_;
}


PosixPath PosixPath::joinpath(const PosixPath& path) const
{
    return PosixPath(base_.joinpath(path.base_));
}


PosixPath PosixPath::joinpath(const string& path) const
{
    return PosixPath(base_.joinpath(path));
}


PosixPath PosixPath::operator/(const PosixPath& path) const
{
    return PosixPath(base_ / path.base_);
}


PosixPath PosixPath::operator/(const string& path) const
{
    return PosixPath(base_ / path);
}


PosixPath& PosixPath::operator/=(const PosixPath& other)
{
    base_ /= other.base_;
    return *this;
}


PosixPath& PosixPath::operator/=(const std::string& other)
{
    base_ /= other;
    return *this;
}


PosixPath PosixPath::parent() const
{
    return PosixPath(base_.parent());
}


vector<PosixPath> PosixPath::parents() const
{
    vector<PosixPath> result;
    for (const auto& path: base_.parents()) {
        // Convert each item to the correct type.
        result.emplace_back(PosixPath(path));
    }
    return result;
}

PosixPath PosixPath::relative_to(const PosixPath& other) const
{
    return PosixPath(base_.relative_to(other.base_));
}


PosixPath PosixPath::with_name(const std::string& name) const
{
    return PosixPath(base_.with_name(name));
}

PosixPath PosixPath::with_suffix(const std::string& name) const
{
    return PosixPath(base_.with_suffix(name));
}


// Implement the PosixPath-specific API.

PosixPath PosixPath::cwd()
{
    return PosixPath(getcwd());
}


PurePosixPath PosixPath::pure() const
{
    // This is not part of the Python API, but is useful here because the
    // private inheritance relationship between PosixPath and PurePosixPath
    // otherwise prevents interoperability.
    return base_;
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


bool PosixPath::is_symlink() const
{
    return path::islink(string(*this));
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


void PosixPath::mkdir(mode_t mode, bool parents, bool exist_ok) const
{
    if (not parents and not parent().is_dir()) {
        throw runtime_error("no such directory: " + string(parent()));
    }
    makedirs(string(*this), mode, exist_ok);
    return;
}


void PosixPath::symlink_to(const PosixPath& target) const
{
    symlink_to(string(target));
    return;
}


void PosixPath::symlink_to(const string& target) const
{
    const string path(*this);
    if (symlink(target.c_str(), path.c_str()) != 0) {
        throw runtime_error(string(strerror(errno)) + ": " + path);
    }
    return;
}


void PosixPath::unlink() const
{
    const string path(*this);
    if (remove(path.c_str()) != 0) {
        throw runtime_error(string(strerror(errno)) + ": " + path);
    }
    return;
}


void PosixPath::rmdir() const
{
    const string path(*this);
    if (::rmdir(path.c_str()) != 0) {
        throw runtime_error(string(strerror(errno)) + ": " + path);
    }
    return;
}


string PosixPath::read_bytes() const
{
    return read_file("rb");
}


string PosixPath::read_text() const
{
    return read_file("rt");
}


void PosixPath::write_bytes(const string& data) const
{
    write_file(data, "wb");
    return;
}


void PosixPath::write_text(const string& data) const
{
    write_file(data, "wt");
    return;
}


string PosixPath::read_file(const string& mode) const
{
    auto stream(open(mode));
    ostringstream buffer;
    if (not (buffer << stream.rdbuf())) {
        throw runtime_error("could not read data from " + string(*this));
    }
    return buffer.str();  // is this a move (good) or copy (bad)?
}


void PosixPath::write_file(const string& data, const string& mode) const
{
    auto stream(open(mode));
    if (not stream.write(data.c_str(), data.size())) {
        throw runtime_error("could not write data to " + string(*this));
    }
    return;
}


vector<PosixPath> PosixPath::iterdir() const
{
    // Implementing this in terms of os::listdir() would require the conversion
    // of a vector<string> to a vector<PosixPath>. This would either double the
    // memory usage or require additional complexity to reduce the size of the
    // original list as the new list is populated. Instead, this is a copy of
    // the os::listdir() implementation with some type-specific tweaks.
    static const vector<string> special({".", ".."});
    const string path(*this);
    errno = 0;  // POSIX requires this to be thread-safe
    auto dir(opendir(path.c_str()));
    if (errno != 0) {
        throw runtime_error(string(strerror(errno)) + ": " + path);
    }
    vector<PosixPath> entries;
    const auto first(begin(special));
    const auto last(end(special));
    dirent* entry;
    while ((entry = readdir(dir))) {
        if (not in(entry->d_name, first, last)) {
            entries.emplace_back(*this / PosixPath(entry->d_name));
        }
    }
    closedir(dir);
    if (errno != 0) {
        throw runtime_error(strerror(errno));
    }
    return entries;
}
