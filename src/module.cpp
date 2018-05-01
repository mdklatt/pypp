/// Implementation of the sample library module.
///
#include "module.hpp"

using pypp::SampleClass;


int SampleClass::add(int x) const
{
    return pypp::add(num, x);
}


int pypp::add(int x, int y)
{
    return x + y;
}
