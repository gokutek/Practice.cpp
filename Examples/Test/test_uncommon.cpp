#include <iostream>
#include "catch.hpp"

/*
=========================
一些不常用的C++特性、关键字
=========================
*/


/*
=========================================================
加上“(std::nothrow)”，内存分配失败时返回NULL，而不是抛出异常
=========================================================
*/
TEST_CASE("nothrow", "nothrow")
{
	char* p = new (std::nothrow) char[0x7fffffff];
	REQUIRE(p == NULL);
}
