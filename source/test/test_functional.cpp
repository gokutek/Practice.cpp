#include <functional>
#include <iostream>
#include "catch.hpp"

/*
==============
decltype的用法
==============
*/


static int function1(int a, int b)
{
	return a - b;
}


TEST_CASE("basic", "[functional]")
{
	std::function<int(int, int)> f = function1;
	int res = f(30, 10);
	REQUIRE(res == 20);
}


TEST_CASE("bind", "[functional]")
{
	int res;

	std::function<int(int)> f1 = std::bind(function1, 30, std::placeholders::_1);
	res = f1(10);
	REQUIRE(res == 20);
	res = f1(60);
	REQUIRE(res == -30);

	std::function<int(int)> f2 = std::bind(function1, std::placeholders::_1, 30);
	res = f2(10);
	REQUIRE(res == -20);
	res = f2(60);
	REQUIRE(res == 30);
}


namespace
{
	class Foo
	{
	public:
		explicit Foo(int val)
			: val_(val)
		{
		}

		int function1(int a, int b) const
		{
			return a - b - val_;
		}

		int val_;
	};
}


TEST_CASE("member function", "[functional]")
{
	int res;

	// 绑定成员函数，&foo改成foo的话，将会拷贝一个对象保存在f1中
	Foo foo(100);
	std::function<int(int, int)> f1 = std::bind(&Foo::function1, &foo, std::placeholders::_1, std::placeholders::_2);
	res = f1(500, 10);
	REQUIRE(res == 390);
}
