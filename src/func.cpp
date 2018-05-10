/// Implementation of the func module.
///
#include <algorithm>
#include "func.hpp"


using std::all_of;
using std::vector;

using namespace pypp;


// Using a vector instead of an iterator range as input allows these functions
// to be called with any arguments that can be used to construct the vector,
// including an iterator range or an initializer list. There's a slight
// performance penalty when using an iterator range because of the unnecessary
// vector construction.

bool func::all(const vector<bool>& items)
{
    // Returns true for an empty sequence, matching Python.
    return all_of(items.begin(), items.end(), [](bool x){ return x; });
}


bool func::any(const vector<bool>& items)
{
    // Returns false for an empty sequence, matching Python.
    return any_of(items.begin(), items.end(), [](bool x){ return x; });
}
