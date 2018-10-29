#if _MSC_VER >= 1914

// 需要VS2017，并且在工程设置里将C++语言标准设置C++17

#include <variant>
#include <string>
#include "catch.hpp"


/*
===============================================================================
std::variant用法：
1.与union的区别；
===============================================================================
*/


namespace
{
	static bool dtor_is_called = false;


	class Foo
	{
	public:
		Foo(int val) : m_val(val)
		{
		}

		~Foo()
		{
			dtor_is_called = true;
		}

		int m_val;
	};
}


TEST_CASE("misc", "[variant]")
{
	std::variant<int, float> v, w;

	v = 12; // v contains int

	int i = std::get<int>(v);
	REQUIRE(i == 12);

	w = std::get<int>(v);
	w = std::get<0>(v); // same effect as the previous line
	w = v; // same effect as the previous line

	//  std::get<double>(v); // error: no double in [int, float]
	//  std::get<3>(v);      // error: valid index values are 0 and 1

	try {
		// warning C4834: 放弃具有 "nodiscard" 属性的函数的返回值
		std::variant<int, float> val = std::get<float>(w); // w contains int, not float: will throw
	} catch (std::bad_variant_access&) {
	}

	std::variant<std::string> vstr("abc"); // converting constructors work when unambiguous
	vstr = "def"; // converting assignment also works when unambiguous
}


/*
===============================================================================
variant中可以放C++对象，并且在覆盖赋值时，会调用该对象的析构函数
===============================================================================
*/
TEST_CASE("object", "[variant]")
{
	REQUIRE(!dtor_is_called);

	std::variant<std::string, Foo> v;
	v = Foo(123);
	v = "hello";
	REQUIRE(dtor_is_called);
}

#endif // _MSC_VER >= 1914
