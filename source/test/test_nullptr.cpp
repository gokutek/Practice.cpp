#include "catch.hpp"



static int foo(int)
{
	return 1;
}


static int foo(char*)
{
	return 2;
}


TEST_CASE("nullptr VS NULL", "[nullptr]")
{
	REQUIRE(foo(NULL) == 1);
	REQUIRE(foo(nullptr) == 2);
}
