#include <initializer_list>
#include "catch.hpp"


namespace
{
	class Foo
	{
	public:
		Foo(int x, int y)
			: x_(x)
			, y_(y)
		{

		}

		int x_, y_;
	};
}


static int GetX(Foo const &foo)
{
	return foo.x_;
}


static int GetY(Foo const &foo)
{
	return foo.y_;
}


/*
================================
增加这个重载后，会优先匹配这个函数
================================
*/
static int GetY(std::initializer_list<int> const &initl)
{
	return 666;
}


TEST_CASE("ctor", "initializer_list")
{
	Foo foo{ 10,20 };
	REQUIRE(foo.x_ == 10);
	REQUIRE(foo.y_ == 20);
	REQUIRE(GetX({ 99,88 }) == 99);
	REQUIRE(GetY({ 99,88 }) == 666);
}
