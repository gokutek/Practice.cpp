#include <bitset>
#include <iostream>
#include "catch.hpp"


TEST_CASE("bitset", "[bitset]")
{
    std::bitset<32> bs(0xfe); // 1111 1110
    REQUIRE(bs.count() == 7);
    REQUIRE(bs.to_string() == "00000000000000000000000011111110");
}
