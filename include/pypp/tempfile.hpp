/// Tools for working wih temporary files and directories.
///
/// @file
#ifndef PYPP_TEMPFILE_HPP
#define PYPP_TEMPFILE_HPP

#include <string>


namespace pypp { namespace tempfile {

/// Return the directory used for temporary files.
///
/// Environment variables will be queried to determine the system temporary
/// directory. Failing that, several standard directories will be searched for.
/// As a last resort, the current working directory is returned.
///
/// @return directory path
std::string gettempdir();


/// Create a unique temporary directory.
///
/// The directory and its contents will automatically be deleted when the
/// object is destroyed.
///
class TemporaryDirectory
{
public:
    /// Directory name.
    ///
    const std::string name;

    /// Construct the object.
    ///
    /// @param suffix
    /// @param prefix
    /// @param dir optional temporary directory
    TemporaryDirectory(const std::string& suffix="", const std::string& prefix="tmp", std::string dir="");

    /// Delete the contents of this directory.
    ///
    void cleanup() const;

    /// Destroy the object.
    ///
    ~TemporaryDirectory();

private:
    /// Delete the contents of a directory.
    ///
    /// @param path directory path
    /// @param delpath delete directory itself
    static void cleanup(const std::string& path, bool delpath=false);
};

}}

#endif  // PYPP_TEMPFILE_HPP
