#include <string>
#include <string_view>
#include "catch.hpp"


TEST_CASE("string", "string")
{
}


/*
===============================================================================
string_view
===============================================================================
*/
TEST_CASE("string_view", "string_view")
{
	static char const *str = "hello";
	std::string_view sv(str);

	std::string::size_type pos = sv.find('e');
	REQUIRE(pos == 1);

	char const *p = &sv.at(0);
	REQUIRE(p == str);
}
