#include <vector>
#include "catch.hpp"

/*
===============================================================================
catch单元测试库的使用：
1. https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md；
2. 可以在工程设置里将[C/C++]、[预处理器]中的预处理到文件打开，看宏展开后的代码；
3. 为每个TEST CASE生成唯一函数名：__COUNTER__；
4. https://github.com/catchorg/Catch2/blob/master/docs/own-main.md；
===============================================================================
*/


TEST_CASE("vectors can be sized and resized", "[vector]")
{
	std::vector<int> v(5);

	REQUIRE(v.size() == 5);
	REQUIRE(v.capacity() >= 5);

	SECTION("resizing bigger changes size and capacity")
	{
		v.resize(10);

		REQUIRE(v.size() == 10);
		REQUIRE(v.capacity() >= 10);
	}
	SECTION("resizing smaller changes size but not capacity")
	{
		v.resize(0);

		REQUIRE(v.size() == 0);
		REQUIRE(v.capacity() >= 5);
	}
	SECTION("reserving bigger changes capacity but not size")
	{
		v.reserve(10);

		REQUIRE(v.size() == 5);
		REQUIRE(v.capacity() >= 10);
	}
	SECTION("reserving smaller does not change size or capacity")
	{
		v.reserve(0);

		REQUIRE(v.size() == 5);
		REQUIRE(v.capacity() >= 5);
	}
}
