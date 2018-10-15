#if _MSC_VER >= 1914

// 需要VS2017，并且在工程设置里将C++语言标准设置C++17

#include <optional>
#include <string>
#include "catch.hpp"


/*
=================
std::optional用法
=================
*/


static std::optional<std::string> create(bool b)
{
	if (b) {
		return "Godzilla";
	} else {
		return {};
	}
}


TEST_CASE("value", "[optional]")
{
	auto str1 = create(false);
	REQUIRE(str1.value_or("foobar") == "foobar");

	auto str2 = create(true);
	REQUIRE(str2.value() == "Godzilla");
}


TEST_CASE("operator->, operator*", "[optional]")
{
	std::optional<int> opt1 = 1;
	REQUIRE(*opt1 == 1);

	std::optional<std::string> opt2 = "abc";
	REQUIRE(opt2->size() == 3);
}


TEST_CASE(":operator bool, has_value", "[optional]")
{
	std::optional<int> opt1 = 1;
	REQUIRE(opt1);
	REQUIRE(opt1.has_value());

	std::optional<std::string> opt2;
	REQUIRE(!opt2);
	REQUIRE(!opt2.has_value());
}

#endif // _MSC_VER >= 1914
