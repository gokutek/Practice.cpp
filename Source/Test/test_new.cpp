#include <iostream>
#include "catch.hpp"


struct Foo
{
	Foo()
	{
		//std::cout << __FUNCTION__ << std::endl;
	}

	~Foo()
	{
		//std::cout << __FUNCTION__ << std::endl;
	}
};


/*
===============================================================================
placement new可以带多个参数，在调用时使用new(...) Object()
===============================================================================
*/
void* operator new(size_t s, char const* file, int line)
{
	//std::cout << __FUNCTION__ << std::endl;
	return malloc(s);
}


void operator delete(void* p, char const* file, int line)
{
	//std::cout << __FUNCTION__ << std::endl;
	free(p);
}


TEST_CASE("placement new", "[new]")
{
	Foo* pFoo = new(__FUNCTION__, __LINE__) Foo();
	delete pFoo;
}
