#include <string>
#include <string_view>
#include "catch.hpp"


TEST_CASE("find, substr", "string")
{
	std::string const str = "string,int";
	size_t pos = str.find(',');
	REQUIRE(std::string::npos != pos);

	std::string const prefix = str.substr(0, pos);
	REQUIRE(prefix == "string");

	std::string const surfix = str.substr(pos + 1);
	REQUIRE(surfix == "int");
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
