#include <assert.h>
#include <iostream>
#include "serialization.h"

struct Person
{
    std::string name;
    int         age;
};


SERIALIZATION(Person, name, age)


int main()
{
    int a = 10;
    int b = 11;
    int ab = 12;
    int c = BOOST_PP_CAT(a, b);
    assert(c == ab);

    int x = SERIALIZATION_PP_VARIADIC_SIZE(1, 2, 3);
    assert(x == 3);
    
    x = SERIALIZATION_PP_VARIADIC_SIZE(1);
    assert(x == 1);

    x = SERIALIZATION_PP_VARIADIC_SIZE();
    assert(x == 1);

    auto vec = GetSerialiation();
    return 0;
}

/*
===============================================================================
1. 模板是编译时的多态；
2. 接口是运行时的多态；
3. 
===============================================================================
*/
