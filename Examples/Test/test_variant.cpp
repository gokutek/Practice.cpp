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

		Foo(Foo const &other) : m_val(other.m_val)
		{
		}

		~Foo()
		{
			dtor_is_called = true;
		}

		int m_val;
	};
}


/*
===============================================================================
0.std::get模板参数可以是索引或者类型；
1.索引越界，或者类型不存在都会有编译错误；
2.当variant中当前实际储存的类型与std::get不一致时，会抛出异常；
===============================================================================
*/
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

	bool has_exception = false;
	try {
		std::variant<int, float> val = std::get<float>(w); // w contains int, not float: will throw
	} catch (std::bad_variant_access&) {
		has_exception = true;
	}
	REQUIRE(has_exception);

	has_exception = false;
	try {
		w = 1.23f;
		std::variant<int, float> val = std::get<float>(w);
	} catch (std::bad_variant_access&) {
		has_exception = true;
	}
	REQUIRE(!has_exception);

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


/*
===============================================================================
std::get返回的实际上是引用类型
===============================================================================
*/
TEST_CASE("get", "[variant]")
{
	std::variant<std::string, Foo> v;
	v = Foo(123);
	Foo &foo = std::get<Foo>(v);
	REQUIRE(foo.m_val == 123);

	std::get<Foo>(v).m_val = 666;
	REQUIRE(foo.m_val == 666);
}

#endif // _MSC_VER >= 1914
