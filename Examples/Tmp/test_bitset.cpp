#include <bitset>
#include <iostream>
#include "catch.hpp"

/*
===================
“位运算”
===================
*/

TEST_CASE("bitset", "[bitset]")
{
    std::bitset<32> bs(0xfe); // 1111 1110
    REQUIRE(bs.count() == 7);
    REQUIRE(bs.to_string() == "00000000000000000000000011111110");

    REQUIRE(bs.size() == 32);

    REQUIRE(!bs.test(0));

    bs.set(0, true);
    REQUIRE(bs.test(0));

    bs.reset();
    REQUIRE(bs.to_string() == "00000000000000000000000000000000");

    bs.set();
    REQUIRE(bs.to_string() == "11111111111111111111111111111111");
}
