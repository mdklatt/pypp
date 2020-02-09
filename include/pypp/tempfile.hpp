/// Tools for working wih temporary files and directories.
///
/// @file
#ifndef PYPP_TEMPFILE_HPP
#define PYPP_TEMPFILE_HPP

#include <string>
#include "pypp/path.hpp"


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
    /// @return: path
    std::string name() const;

    /// Construct the object.
    ///
    /// Note that, unlike its Python counterpart, this does not have a 'suffix'
    /// argument.
    ///
    /// @param prefix directory prefix
    /// @param dir optional temporary directory
    explicit TemporaryDirectory(const std::string& prefix="tmp", std::string dir="");

    /// Delete the contents of this directory.
    ///
    void cleanup() const;

    /// Destroy the object.
    ///
    ~TemporaryDirectory();

private:
    /// Recursively remove a directory tree.
    ///
    /// @param root: root directory
    /// @param delpath: delete root directory itself
    static void rmtree(const path::Path& root, bool delroot=false);

    /// @property
    path::Path path;

};

}}

#endif  // PYPP_TEMPFILE_HPP
