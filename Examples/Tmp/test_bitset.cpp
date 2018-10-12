#include <bitset>
#include <iostream>
#include "catch.hpp"


TEST_CASE("bitset", "[bitset]")
{
    std::bitset<32> bs(0xfe);
    std::cout << bs.count() << std::endl;
    std::cout << bs.to_string() << std::endl;
}
