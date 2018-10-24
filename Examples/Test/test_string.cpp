#include <string>
#include <string_view>
#include "catch.hpp"


TEST_CASE("find, substr", "std::string")
{
	std::string const str = "hello,world";
	size_t pos = str.find(',');
	REQUIRE(std::string::npos != pos);

	std::string const prefix = str.substr(0, pos);
	REQUIRE(prefix == "hello");

	std::string const surfix = str.substr(pos + 1);
	REQUIRE(surfix == "world");

	pos = str.find("hello");
	REQUIRE(std::string::npos != pos);
	REQUIRE(0 == pos);

	pos = str.find("Fuck");
	REQUIRE(std::string::npos == pos);
}


TEST_CASE("find_first_of", "std::string")
{
	std::string const str = "hello,world";
	size_t pos = str.find_first_of("egg");
	REQUIRE(pos == 1);

	pos = str.find_first_of("fuck");
	REQUIRE(pos == std::string::npos);
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
