static int i = __COUNTER__;
static int j = __COUNTER__;
static int k = __COUNTER__;


#include "catch.hpp"


TEST_CASE("COUNTER", "[COUNTER]")
{
    REQUIRE(i == 0);
    REQUIRE(j == 1);
    REQUIRE(k == 2);
}

/*
===============================================================================
在每个编译单元（cpp），__COUNTER__都是从0开始计数，也就是说不同的cpp中，
__COUNTER__会出现相同的值
===============================================================================
*/
