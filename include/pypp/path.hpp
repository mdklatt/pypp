/// Common file path manipulations.
///
/// This combines functionality from the Python os.path and pathlib modules.
///
/// @file
#ifndef PYPP_PATH_HPP
#define PYPP_PATH_HPP

#include <string>
#include <utility>
#include <vector>


namespace pypp { namespace path {

/// Path separator.
///
/// In the current implementation this is not OS-specific, but works for both
/// *nix and Windows.
///
extern const std::string sep;


/// Join path segments into a complete path.
///
/// Use an empty string as the last to segment to ensure that the path ends in
/// a trailing separator.
///
/// @param parts individual path parts
/// @return complete path
std::string join(const std::vector<std::string>& parts);


/// Split a path into directory and name components.
///
/// If the path has a trailing separator, the pathname component will be empty.
/// Calling join() on the resulting segments will return an equivalent (but not
/// necessarily identical) path.
///
/// @param path input path
/// @return (root, name) pair
std::pair<std::string, std::string> split(const std::string& path);


/// Get the directory component from a path.
///
/// This is identical to the first value of the result returned by split().
///
/// @param path input
/// @return directory component
std::string dirname(const std::string& path);


/// Get the name component from a path.
///
/// This is identical to the second value of the result returned by split().
///
/// @param path input path
/// @return name component
std::string basename(const std::string& path);


/// Normalize a path.
///
/// @param path input path
/// @return normalized path
std::string normpath(const std::string& path);


/// Return an absolute path.
///
/// The is this normalized version of the path joined with the current working
/// directory.
///
/// @param path input path
/// @return
std::string abspath(const std::string& path);


/// Determine if a path is absolute.
///
/// @param path input path
/// @return true for an absolute path
bool isabs(const std::string& path);


/// Split a path into a root and an extension.
///
/// @param path input path
/// @return (root, extension) pair
std::pair<std::string, std::string> splitext(const std::string& path);


/// Determine if a path exists.
///
/// If the path is a link its target is tested.
///
/// @param path input path
/// @return true if path exists
bool exists(const std::string& path);


/// Determine if a path is an existing file.
///
/// If the path is a link its target is tested.
///
/// @param path input path
/// @return true if path is an existing file
bool isfile(const std::string& path);


/// Determine if a path is an existing directory.
///
/// If the path is a link its target is tested.
///
/// @param path input path
/// @return true if path is an existing directory
bool isdir(const std::string& path);


/// System-independent representation of a file path.
///
class PurePath
{
public:
    /// Create a path object.
    ///
    /// @param path path as a string
    PurePath(std::string path=".");

    /// Represent the path as a std::string.
    ///
    /// @return string representation
    explicit operator std::string() const;

    /// Join this path with another path.
    ///
    /// @param other other path to join with
    /// @return new joined path
    PurePath joinpath(const PurePath& other) const;

    /// Join this path with another path.
    ///
    /// @param other path to join with
    /// @return new joned path
    PurePath joinpath(const std::string& other) const;

    /// Join this path with another path
    ///
    /// @param other path to join with
    /// @return new joined path
    PurePath operator/(const std::string& other) const;

    /// Join this path with another path.
    ///
    /// @param other path to join with
    /// @return new joined path
    PurePath operator/(const PurePath& other) const;

    /// Join this path in place with another path.
    ///
    /// @param  path to join with
    /// @return modified path
    PurePath& operator/=(const std::string& other);

    /// Join this path in place with another path.
    ///
    /// @param other path to join with
    /// @return modified path
    PurePath& operator/=(const PurePath& other);

    /// Equality operator.
    ///
    /// @param other path to compare
    /// @return true if paths are equal
    bool operator==(const PurePath& other) const;

    /// Inequality operator
    ///
    /// @param other path to compare
    /// @return true if paths are not equal
    bool operator!=(const PurePath& other) const;

    /// Determine if the path is absolute.
    ///
    /// @return true if this is an absolute path
    bool is_absolute() const;

    /// Get the final path component.
    ///
    /// @return name
    std::string name() const;

    /// Split the path into its component parts.
    ///
    /// @return path parts
    const std::vector<std::string>& parts() const;

    /// Get the path root (`` or `/`).
    ///
    /// @return root
    const std::string root() const;

    /// Compute the direct parent path.
    ///
    /// @return parent path
    PurePath parent() const;

    /// Compute all ancestor paths, starting with the direct parent.
    ///
    /// @return ancestor paths
    std::vector<PurePath> parents() const;

    /// Compute a relative path.
    ///
    /// @param other parent path
    /// @return relative path
    PurePath relative_to(const PurePath& other) const;

    /// Get the path name without its suffix.
    ///
    /// @return stem
    std::string stem() const;

    /// Get the final file extension for the path name.
    ///
    /// @return suffix
    std::string suffix() const;

    /// Get all file extensions for the path name.
    ///
    /// @return
    std::vector<std::string> suffixes() const;

    /// Replace the path name.
    ///
    /// @return new path
    PurePath with_name(const std::string& name) const;

    /// Replace the path suffix.
    ///
    /// @return new path
    PurePath with_suffix(const std::string& suffix) const;

private:
    std::vector<std::string> parts_;

    /// Determine if path is a relative (`.`) or absolute (`/`) root.
    ///
    /// @return true if path is a root
    bool is_root() const;
};


class PosixPath: public PurePath
{

};

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
using Path = PosixPath;
#endif

}}  // namespace

#endif  // PYPP_PATH_HPP
