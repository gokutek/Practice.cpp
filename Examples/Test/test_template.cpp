#include <assert.h>
#include "catch.hpp"


/*
=======
T与引用
=======
*/
template<typename T>
void test_t_type(T val)
{
    val = 55555;
}


void test_t_type_func()
{
    int a = 100;
    int &ra = a;

    // 虽然ra是int&，在到test_t_type中，T被推断为int类型
    test_t_type(ra);
    assert(a == 100);

    //强制将T设为int&
    test_t_type<int&>(ra);
    assert(a == 55555);
}


/*
=========
T&&与引用
=========
*/
template<typename T>
void test_t_type2(T&& val)
{
    val = 666666;
}


TEST_CASE("test_t_type2", "[template]")
{
	int a = 100;
	int &ra = a;
	test_t_type2(ra);
	REQUIRE(a == 666666);
}
