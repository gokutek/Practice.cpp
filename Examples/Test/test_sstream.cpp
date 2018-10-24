#include <sstream>
#include "catch.hpp"

/*
=====================
sstream中相关类的使用
=====================
*/

TEST_CASE("sstream clear", "sstream")
{
	std::ostringstream oss;
	oss << "hello";
	oss.clear(); // clear不是清理掉之前的输入
	oss << "world";

	REQUIRE(oss.str() == "helloworld");
}
