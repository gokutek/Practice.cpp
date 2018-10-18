#include <math.h>
#include <numeric>
#include <limits>
#include <iostream>
#include "catch.hpp"


/*
===============================================================================
float类型的一些tricks：
1.最大值、最小值强转为int后为0；
===============================================================================
*/


TEST_CASE("Float", "Float")
{
#pragma warning(push)
#pragma warning(disable: 4056)
    float const maxFloat = std::numeric_limits<float>::max();
    float const minFloat = std::numeric_limits<float>::min();
    REQUIRE((int)maxFloat == 0);
    REQUIRE((int)minFloat == 0);
#pragma warning(pop)

    float x = -226423088.0f;
    x += 1;
    REQUIRE(x == -226423088.0f);
}
