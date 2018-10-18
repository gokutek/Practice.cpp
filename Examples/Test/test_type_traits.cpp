#include <vector>
#include <type_traits>
#include "catch.hpp"


/*
===============================================================================
type_traits中很多"__is"开头的函数都是编译器内部实现的，看不到它们的代码
===============================================================================
*/


namespace
{
    enum Sex
    {
        BOY,
        GIRL
    };
}


template<class T>
typename std::enable_if<std::is_integral<T>::value, int>::type add(T v1, T v2)
{
    return 1;
}


template<class T>
typename std::enable_if<!std::is_integral<T>::value, int>::type add(T v1, T v2)
{
    return 2;
}


TEST_CASE("type_traits", "[type_traits]")
{
    REQUIRE(1 == add(1, 2));
    REQUIRE(2 == add(std::string("123"), std::string("123")));

    REQUIRE(std::is_void<void>::value);
    
    REQUIRE(std::is_null_pointer<std::nullptr_t>::value);
    
    REQUIRE(std::is_integral<int>::value);
    REQUIRE(std::is_integral<short>::value);
    REQUIRE(std::is_integral<bool>::value);

    REQUIRE(std::is_floating_point<float>::value);
    REQUIRE(std::is_floating_point<double>::value);

    REQUIRE(std::is_enum<Sex>::value);

    REQUIRE(!std::is_polymorphic<int>::value);
    REQUIRE(!std::is_abstract<int>::value);
    REQUIRE(!std::is_empty<int>::value);

    bool bRet = !std::is_base_of<int, long>::value;
    REQUIRE(bRet);

    REQUIRE(!std::is_class<int>::value);
    REQUIRE(!__is_class(int));
    REQUIRE(__is_class(std::vector<int>));
}
