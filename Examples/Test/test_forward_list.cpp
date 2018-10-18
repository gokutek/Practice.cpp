#include <forward_list>
#include "catch.hpp"


TEST_CASE("forward_list", "[std::forward_list]")
{
    std::forward_list<int> l;
    
    l.push_front(1);
    REQUIRE(l.front() == 1);
}
