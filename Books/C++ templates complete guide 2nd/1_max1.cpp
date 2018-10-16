#include <iostream>
#include "catch.hpp"


namespace
{
	struct Foo
	{
		int val;

		Foo(int v) : val(v)
		{
			//std::cout << __FUNCTION__ << std::endl;
		}

		Foo(Foo const &other) : val(other.val)
		{
			//std::cout << __FUNCTION__ << std::endl;
		}

		~Foo()
		{
			//std::cout << __FUNCTION__ << std::endl;
		}

		bool operator<(Foo const &other) const
		{
			return val < other.val;
		}
	};
}


template<typename T>
T max1(T a, T b)
{
	return b < a ? a : b;
}


TEST_CASE("max1.hpp", "basic")
{
	Foo f1(1);
	Foo f2(2);

	// 模板被实例化为：Foo max1(Foo a, Foo b)，所以在调用过程中，会有对象拷贝
	auto res = max1(f1, f2);
	REQUIRE(res.val == 2);
}
