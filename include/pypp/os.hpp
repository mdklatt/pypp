/**
 * Tools for interacting with the operating system.
 *
 * @file
 */
#ifndef PYPP_OS_HPP
#define PYPP_OS_HPP

#include <string>
#include <vector>


namespace pypp { namespace os {


/**
 * Get the current working directory.
 *
 * @return: current working directory
 */
std::string getcwd();


/**
 * Change the current working directory.
 *
 * @param path: new working directory path
 */
void chdir(const std::string& path);


/**
 * List the names in a directory.
 *
 * Special entries (e.g. "." and "..") are ignored.
 *
 * @param path: directory path
 * @return: directory contents
 */
std::vector<std::string> listdir(const std::string& path=".");


/**
 * Recursively make a new directory.
 *
 * @param path: directory path
 * @param mode: directory permissions
 * @param exist_ok: directory must not already exist if false
 */
void makedirs(const std::string& path, mode_t mode=0777, bool exist_ok=false);


/**
 * Recursively remove empty directories.
 *
 * @param path: directory path
 */
void removedirs(const std::string& path);

}}


#endif  // PYPP_OS_HPP
