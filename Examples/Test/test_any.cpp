#include <any>
#include <string>
#include "catch.hpp"


namespace
{
	class Foo
	{
	public:
		Foo(int val) : value(val)
		{
		}

		~Foo()
		{
		}

		int value;
	};
}


TEST_CASE("any", "[std::any]")
{
	std::any a;
	a = std::string("123");

	std::string const str = std::any_cast<std::string>(a);
	REQUIRE(str == "123");

	a = Foo(123);
	REQUIRE(std::any_cast<Foo>(a).value == 123);

	std::any_cast<Foo&>(a).value = 666;
	REQUIRE(std::any_cast<Foo>(a).value == 666);
}
