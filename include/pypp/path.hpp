/// Common file path manipulations.
///
/// This combines functionality from the Python os.path and pathlib modules.
///
/// @file
#ifndef PYPP_PATH_HPP
#define PYPP_PATH_HPP

#include <fstream>
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
/// @param path: input path
/// @return true for an existing file
bool isfile(const std::string& path);


/// Determine if a path is an existing directory.
///
/// If the path is a link its target is tested.
///
/// @param path: input path
/// @return true for an existing directory
bool isdir(const std::string& path);


/// Determine if the path is an existing symbolic link.
///
/// @param path: input path
/// @return true for an existing symbolic link
bool islink(const std::string& path);


/// System-independent representation of a POSIX file path.
///
class PurePosixPath
{
public:
    /// Create a path object.
    ///
    /// @param path path as a string
    explicit PurePosixPath(std::string path=".");

    /// Represent the path as a std::string.
    ///
    /// @return string representation
    explicit operator std::string() const;

    /// Join this path with another path.
    ///
    /// @param other other path to join with
    /// @return new joined path
    PurePosixPath joinpath(const PurePosixPath& other) const;

    /// @overload
    PurePosixPath joinpath(const std::string& other) const;

    /// Join this path with another path
    ///
    /// @param other path to join with
    /// @return new joined path
    PurePosixPath operator/(const std::string& other) const;

    /// @overload
    PurePosixPath operator/(const PurePosixPath& other) const;

    /// Join this path in place with another path.
    ///
    /// @param  path to join with
    /// @return modified path
    PurePosixPath& operator/=(const std::string& other);

    /// @overload
    PurePosixPath& operator/=(const PurePosixPath& other);

    /// Equality operator.
    ///
    /// @param other path to compare
    /// @return true if path is equal to other
    bool operator==(const PurePosixPath& other) const;

    /// Inequality operator.
    ///
    /// @param other path to compare
    /// @return true if path is not equal to other
    bool operator!=(const PurePosixPath& other) const;

    /// Less-than operator.
    ///
    /// This a lexical comparison and does not imply anything about directory
    /// hierarchies. This is mainly intended to allow the use of PurePosixPath
    /// objects in contexts that require a sort order.
    ///
    /// @param other path to compare
    /// @return true if path is less than other
    bool operator<(const PurePosixPath& other) const;

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
    std::string root() const;

    /// Compute the direct parent path.
    ///
    /// @return parent path
    PurePosixPath parent() const;

    /// Compute all ancestor paths, starting with the direct parent.
    ///
    /// @return ancestor paths
    std::vector<PurePosixPath> parents() const;

    /// Compute a relative path.
    ///
    /// @param other parent path
    /// @return relative path
    PurePosixPath relative_to(const PurePosixPath& other) const;

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
    PurePosixPath with_name(const std::string& name) const;

    /// Replace the path suffix.
    ///
    /// @return new path
    PurePosixPath with_suffix(const std::string& suffix) const;

private:
    std::vector<std::string> parts_;

    /// Determine if path is a relative (`.`) or absolute (`/`) root.
    ///
    /// @return true if path is a root
    bool is_root() const;
};


///
///
class PosixPath
{
public:
    /// Represent the path as a std::string.
    ///
    /// @return string representation
    explicit operator std::string() const;

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
    std::string root() const;

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

    /// Get the current working directory.
    ///
    /// @return path of the current working directory
    static PosixPath cwd();

    /// Create a path object.
    ///
    /// @param path path as a string
    explicit PosixPath(const std::string& path=".");

    /// Copy constructor.
    ///
    /// @param path
    explicit PosixPath(PurePosixPath  path);

    /// Join this path with another path.
    ///
    /// @param path other path to join with
    /// @return new joined path
    PosixPath joinpath(const PosixPath& path) const;

    /// @overload
    PosixPath joinpath(const std::string& path) const;

    /// Join this path with another path,
    ///
    /// @param path path to join with
    /// @return new joined path
    PosixPath operator/(const std::string& path) const;

    /// @return overload
    PosixPath operator/(const PosixPath& other) const;

    /// Join this path in place with another path.
    ///
    /// @param other path to join with
    /// @return modified path
    PosixPath& operator/=(const std::string& other);

    /// @overload
    PosixPath& operator/=(const PosixPath& other);

    /// Equality operator.
    ///
    /// @param other path to compare
    /// @return true if path is equal to other
    bool operator==(const PosixPath& other) const;

    /// Inequality operator.
    ///
    /// @param other path to compare
    /// @return true if path is not equal to other
    bool operator!=(const PosixPath& other) const;

    /// Less-than operator.
    ///
    /// This a lexical comparison and does not imply anything about directory
    /// hierarchies. This is mainly intended to allow the use of PosixPath
    /// objects in contexts that require a sort order.
    ///
    /// @param other path to compare
    /// @return true if path is less than other
    bool operator<(const PosixPath& other) const;

    /// Compute the direct parent path.
    ///
    /// @return parent path
    PosixPath parent() const;

    /// Compute all ancestor paths, starting with the direct parent.
    ///
    /// @return ancestor paths
    std::vector<PosixPath> parents() const;

    /// Compute a relative path.
    ///
    /// @param other parent path
    /// @return relative path
    PosixPath relative_to(const PosixPath& other) const;

    /// Replace the path name.
    ///
    /// @return new path
    PosixPath with_name(const std::string& name) const;

    /// Replace the path suffix.
    ///
    /// @return new path
    PosixPath with_suffix(const std::string& suffix) const;

    /// Convert to a PurePath object.
    ///
    PurePosixPath pure() const;

    /// Test for the existence of the path.
    ///
    /// @return true if this is an existing file or directory.
    bool exists() const;

    /// Test if the path is a valid directory.
    ///
    /// @return true for a directory
    bool is_dir() const;

    /// Test if the path is a valid file.
    ///
    /// @return true for a file
    bool is_file() const;

    /// Test if the path is a valid symbolic link.
    ///
    /// @return true for a symbolic link
    bool is_symlink() const;

    /// Open a file stream for this path.
    ///
    /// Unlike the Python version, an exception will not be thrown if the file
    /// cannot be opened.
    ///
    /// @param mode file mode (follows Python conventions)
    /// @return open file stream
    std::fstream open(const std::string& mode="rt") const;

    /// Create a directory at this path.
    ///
    /// @param mode permissions for new leaf directory
    /// @param parents create missing parents as needed
    /// @param exist_ok no error if directory already exists
    void mkdir(mode_t mode=0777, bool parents=false, bool exist_ok=false) const;

    /// Create a symbolic link at this path.
    ///
    /// @param target: path to link to
    void symlink_to(const PosixPath& target) const;

    /// @overload
    void symlink_to(const std::string& target) const;

    /// Remove the file with this path.
    ///
    /// Trying to unlink a nonexistent file is an error. Use `rmdir()` to
    /// remove a directory.
    ///
    void unlink() const;

    /// Remove the directory with this path.
    ///
    /// Trying to remove a nonexistent or non-empty directory is an error. Use
    /// `unlink` to remove a file.
    ///
    void rmdir() const;

    /// Read binary data from a file with this path.
    ///
    /// @return binary data
    std::string read_bytes() const;

    /// Read text data from a file with this path.
    ///
    /// Unlike the Python version, this does not support the `encoding` or
    /// `errors` parameters.
    ///
    /// @return file contents
    std::string read_text() const;

    /// Write binary data to a file with this path.
    ///
    /// If the file already exists it will be overwritten.
    ///
    /// @param data: file contents
    void write_bytes(const std::string& data) const;

    /// Write text data to a file with this path.
    ///
    /// If the file already exists it will be overwritten. Unlike the Python
    /// version, this does not support the `enocoding` or `errors` parameters.
    ///
    /// @param data: file contents
    void write_text(const std::string& data) const;

    /// List all items in the directory with this path.
    ///
    /// Unlike Python, this returns a complete sequence, not a generator.
    ///
    /// @return
    std::vector<PosixPath> iterdir() const;

private:
    PurePosixPath base_;

    /// Read file contents.
    ///
    /// @param mode file mode
    /// @return file contents
    std::string read_file(const std::string& mode) const;

    /// Write file contents.
    ///
    /// @param mode file mode
    /// @param data file contents
    void write_file(const std::string& data, const std::string& mode) const;
};


#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
using Path = PosixPath;
using PurePath = PurePosixPath;
#else
#error "path module requires *nix"
#endif

}}  // namespace

#endif  // PYPP_PATH_HPP
