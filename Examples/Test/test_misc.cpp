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
#ifdef WIN32
	char* p = new (std::nothrow) char[0x7fffffff];
    REQUIRE(p == NULL);
#endif // WIN32
}


/*
===============================================================================
将析构函数声明为纯虚函数仅在一种情况下有用：这个类没有其它纯虚接口。
此时，将析构函数声明为纯虚函数，可以阻止该类型的实例化。
===============================================================================
*/
namespace
{
	class Animal
	{
	public:
		virtual ~Animal() = 0;
	};


	Animal::~Animal()
	{
	}


	class Cat : public Animal
	{

	};
}


TEST_CASE("pure virtual dtor", "dtor")
{
	//Animal ani;
}
