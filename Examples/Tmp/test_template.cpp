#include <assert.h>
#include "catch.hpp"


template<typename T>
void test_t_type(T val)
{
    val = 10;
}


void test_t_type_func()
{
    int a = 100;
    int& ra = a;
    // 虽然ra是int&，在到test_t_type中，T被推断为int类型
    test_t_type(ra);
    assert(ra == 100);
    //强制将T设为int&
    test_t_type<int&>(ra);
    assert(ra == 10);
}


template<typename T>
void test_t_type2(T&& val)
{
    val = 10;
}


void test_t_type_func2()
{
    int a = 100;
    int& ra = a;
    test_t_type2(ra);
    assert(ra == 10);
}


TEST_CASE("function template", "[template]")
{
    test_t_type_func();
    test_t_type_func2();
}
