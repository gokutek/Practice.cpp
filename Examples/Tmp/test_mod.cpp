#include "catch.hpp"


TEST_CASE("mod", "[mod]")
{
    REQUIRE(10 % 3 == 1);
    REQUIRE(-10 % 3 == -1);
    REQUIRE(10 % -3 == 1);
    REQUIRE(-10 % -3 == -1);
}
