#include <iostream>
#include "catch.hpp"


namespace
{
	int g_ctor_called = 0;	// 构造函数调用次数
	int g_cpor_called = 0;	// 拷贝构造函数调用次数


	struct Foo
	{
		int val;

		Foo(int v) : val(v)
		{
			++g_ctor_called;
		}

		Foo(Foo const &other) : val(other.val)
		{
			++g_cpor_called;
		}

		~Foo()
		{
		}

		bool operator<(Foo const &other) const
		{
			return val < other.val;
		}
	};
}


/*
===============================================================================
需要思考的问题：
1.参数是否会拷贝？
===============================================================================
*/
template<typename T>
T max1(T a, T b)
{
	return b < a ? a : b;
}


TEST_CASE("max1", "basic")
{
	Foo f1(1);
	Foo f2(2);

	REQUIRE(2 == g_ctor_called);
	REQUIRE(0 == g_cpor_called);

	// 模板被实例化为：Foo max1(Foo a, Foo b)，函数参数会有对象拷贝
	Foo res = max1(f1, f2);
	REQUIRE(res.val == 2);
	REQUIRE(2 == g_ctor_called);
	REQUIRE(3 == g_cpor_called);

	// 非主流用法：强制实例化为Foo const&，函数参数不会有对象拷贝
	Foo const& res1 = max1<Foo const&>(f1, f2);
	REQUIRE(res1.val == 2);
	REQUIRE(2 == g_ctor_called);
	REQUIRE(3 == g_cpor_called);
}


/*
===============================================================================
改进max1，避免函数参数的对象拷贝
===============================================================================
*/
template<typename T>
T max2(T const &a, T const &b)
{
	return b < a ? a : b;
}


TEST_CASE("max2", "basic")
{
	g_ctor_called = 0;
	g_cpor_called = 0;

	Foo f1(1);
	Foo f2(2);

	REQUIRE(2 == g_ctor_called);
	REQUIRE(0 == g_cpor_called);

	Foo res1 = max2(f1, f2);

	REQUIRE(2 == g_ctor_called);
	REQUIRE(1 == g_cpor_called); // 这里的一次拷贝是函数返回值
}


template<typename T>
int f(T = "")
{
	return 111;
}


TEST_CASE("f3", "basic")
{
	// 这也可以，调用f<int>。但是直接定义“int f(int = "")”这样的函数是编译不过的
	REQUIRE(111 == f(0));
}
