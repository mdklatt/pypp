/**
 * Verify that the library is usable in an external CMake project by testing
 * that all headers are present and all modules are linkable.
 */
#include <cassert>
#include <iostream>
#include <string>
#include "pypp/pypp.hpp"


/**
 * Execute the application.
 *
 * @return: exit status
 */
int main() {
    std::string str("abc");
    assert(pypp::str::upper(str) == "ABC");  // TODO: don't use assert()
    std::cout << "libtest succeeded" << std::endl;
    return 0;
}
