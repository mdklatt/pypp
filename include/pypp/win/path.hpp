/**
 * Windows-specific implementations of the 'path' module.
 *
 * @file
 */
#ifndef PYPP_WIN_PATH_HPP
#define PYPP_WIN_PATH_HPP

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include "pypp/path.hpp"


namespace pypp { namespace path {

///
///
class WindowsPath
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
    static WindowsPath cwd();

    /// Create a path object.
    ///
    /// @param path path as a string
    explicit WindowsPath(const std::string& path=".");

    /// Copy constructor.
    ///
    /// @param path
    explicit WindowsPath(PureWindowsPath  path);

    /// Join this path with another path.
    ///
    /// @param path other path to join with
    /// @return new joined path
    WindowsPath joinpath(const WindowsPath& path) const;

    /// @overload
    WindowsPath joinpath(const std::string& path) const;

    /// Join this path with another path,
    ///
    /// @param path path to join with
    /// @return new joined path
    WindowsPath operator/(const std::string& path) const;

    /// @return overload
    WindowsPath operator/(const WindowsPath& other) const;

    /// Join this path in place with another path.
    ///
    /// @param other path to join with
    /// @return modified path
    WindowsPath& operator/=(const std::string& other);

    /// @overload
    WindowsPath& operator/=(const WindowsPath& other);

    /// Equality operator.
    ///
    /// @param other path to compare
    /// @return true if path is equal to other
    bool operator==(const WindowsPath& other) const;

    /// Inequality operator.
    ///
    /// @param other path to compare
    /// @return true if path is not equal to other
    bool operator!=(const WindowsPath& other) const;

    /// Less-than operator.
    ///
    /// This a lexical comparison and does not imply anything about directory
    /// hierarchies. This is mainly intended to allow the use of WindowsPath
    /// objects in contexts that require a sort order.
    ///
    /// @param other path to compare
    /// @return true if path is less than other
    bool operator<(const WindowsPath& other) const;

    /// Compute the direct parent path.
    ///
    /// @return parent path
    WindowsPath parent() const;

    /// Compute all ancestor paths, starting with the direct parent.
    ///
    /// @return ancestor paths
    std::vector<WindowsPath> parents() const;

    /// Compute a relative path.
    ///
    /// @param other parent path
    /// @return relative path
    WindowsPath relative_to(const WindowsPath& other) const;

    /// Replace the path name.
    ///
    /// @return new path
    WindowsPath with_name(const std::string& name) const;

    /// Replace the path suffix.
    ///
    /// @return new path
    WindowsPath with_suffix(const std::string& suffix) const;

    /// Convert to a PurePath object.
    ///
    PureWindowsPath pure() const;

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
    void symlink_to(const WindowsPath& target) const;

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
    std::vector<WindowsPath> iterdir() const;

private:
    PureWindowsPath base_;

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

}}  // namespace pypp::path

#endif  // PYPP_WIN_PATH_HPP
