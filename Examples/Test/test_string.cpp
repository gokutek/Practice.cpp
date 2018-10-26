#include <map>
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


TEST_CASE("data", "std::string")
{
	std::string const str = "hello,world";
	REQUIRE(str.data() == str.c_str());
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


/*
===============================================================================
设计接口时，参数用std::string const&还是char const*：
下面这种情况，可能是用std::string const&比较好。如果用char const*，在调用find时，
肯定会引发std::string的构造。用std::string const&，就要看调用者怎么传参数了，如
果调用者传的就是std::string const&，就避免了std::string的内存分配；
===============================================================================
*/
static bool find_key(std::string const &key)
{
	std::map<std::string, int> m;
	return m.find(key) != m.end();
}
