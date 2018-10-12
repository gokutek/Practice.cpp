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
