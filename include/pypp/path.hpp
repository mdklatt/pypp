/**
 * Common file path manipulations.
 *
 * This combines functionality from the Python os.path and pathlib modules.
 *
 * @file
 */
#ifndef PYPP_PATH_HPP
#define PYPP_PATH_HPP


namespace pypp { namespace path {

/**
 * Path separator.
 */
extern const std::string sep;  // platform-specific


/**
 * Join path segments into a complete path.
 * 
 * Use an empty string as the last to segment to ensure that the path ends in a
 * trailing separator.
 * 
 * @param parts individual path parts
 * @return complete path
 */
std::string join(const std::vector<std::string>& parts);


/**
 * Split a path into directory and name components.
 * 
 * If the path has a trailing separator, the pathname component will be empty.
 * Calling join() on the resulting segments will return an equivalent (but not
 * necessarily identical) path.
 * 
 * @param path input path
 * @return (root, name) pair
 */
std::pair<std::string, std::string> split(const std::string& path);


/**
 * Get the directory component from a path.
 * 
 * This is identical to the first value of the result returned by split().
 * 
 * @param path input
 * @return directory component
 */
std::string dirname(const std::string& path);


/**
 * Get the name component from a path.
 * 
 * This is identical to the second value of the result returned by split().
 * 
 * @param path input path
 * @return name component
 */
std::string basename(const std::string& path);


/**
 * Normalize a path.
 * 
 * @param path input path
 * @return normalized path
 */
std::string normpath(const std::string& path);


/**
 * Return an absolute path.
 * 
 * The is this normalized version of the path joined with the current working
 * directory.
 * 
 * @param path input path
 * @return absolute path
 */
std::string abspath(const std::string& path);


/**
 * Determine if a path is absolute.
 * 
 * @param path input path
 * @return true for an absolute path
 */
bool isabs(const std::string& path);


/**
 * Split a path into a root and an extension.
 * 
 * @param path input path
 * @return (root, extension) pair
 */
std::pair<std::string, std::string> splitext(const std::string& path);


/**
 * Determine if a path exists.
 * 
 * If the path is a link its target is tested.
 * 
 * @param path input path
 * @return true if path exists
 */
bool exists(const std::string& path);


/**
 * Determine if a path is an existing file.
 * 
 * If the path is a link its target is tested.
 * 
 * @param path: input path
 * @return true for an existing file
 */
bool isfile(const std::string& path);


/**
 * Determine if a path is an existing directory.
 * 
 * If the path is a link its target is tested.
 * 
 * @param path: input path
 * @return true for an existing directory
 */
bool isdir(const std::string& path);


/**
 * Determine if the path is an existing symbolic link.
 * 
 * @param path: input path
 * @return true for an existing symbolic link
 */
bool islink(const std::string& path);


/**
 * Base class for system-independent path representations.
 */
class PureBasePath {
public:
    /**
     * Represent the path as a string.
     *
     * @return string representation
     */
    explicit operator std::string() const;

    /**
     * Determine if the path is absolute.
     *
     * @return true if this is an absolute path
     */
    bool is_absolute() const;

    /**
     * Get the final path component.
     *
     * @return name
     */ 
    std::string name() const;

    /**
     * Split the path into its component parts.
     *
     * @return path parts
     */
    const std::vector<std::string>& parts() const { return parts_; }

    /**
     * Get the path root.
     *
     * @return root
     */
    std::string root() const;

    /**
     * Get the path name without its suffix.
     *
     * @return stem
     */
    std::string stem() const;

    /**
     * Get the final file extension for the path name.
     *
     * @return suffix
     */
    std::string suffix() const;

    /**
     * Get all file extensions for the path name.
     *
     * @return list of suffixes
     */
    std::vector<std::string> suffixes() const;

protected:
    /** @property path components */
    std::vector<std::string> parts_;

    /**
     * Create a path object.
     *
     * @param path path as a string
     */
    explicit PureBasePath(std::string path=".");

    /**
     * Lexical path comparison.
     *
     * This can be use to implement comparison operators. Lexical ordering does
     * not imply anything about directory hierarchies. Comparison of
     * heterogeneous child classes may or may not be meaningful.
     *
     * @return: 0 for equality, -1 for less than, 1 for greater than
     */
    short compare(const PureBasePath& other) const;

    /**
     * Determine if path is a relative (`.`) or absolute root.
     *
     * @return true if path is a root
     */
    bool is_root() const;
};

}}  // namespace pypp::path


#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "pypp/posix/path.hpp"
namespace pypp { namespace path {

using PurePath = pypp::path::PurePosixPath;
using Path = pypp::path::PosixPath;

}}  // namespace pypp::path

#elif defined (_WIN32)
#error "path module requires POSIX"
// TODO: #include "pypp/win/path.hpp"
#else
#error "path module requires POSIX"
#endif

#endif  // PYPP_PATH_HPP
