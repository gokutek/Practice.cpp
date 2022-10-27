#include <math.h>
#include <numeric>
#include <limits>
#include <iostream>
#include "catch.hpp"


class Magic
{
    double x;
public:
    friend std::ostream& operator<<(std::ostream& s, Magic c)
    {
        return s;
    }
};


TEST_CASE("Float", "04737")
{
}
