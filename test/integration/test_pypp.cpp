/**
 * Integration test for the 'pypp' library.
 */
#include <cassert>
#include <string>
#include <pypp/string.hpp>


/**
 * Execute the application.
 */
int main() {
    std::string str("abc");
    assert(pypp::str::upper(str) == "ABC");  // TODO: don't use assert()
    return 0;
}
