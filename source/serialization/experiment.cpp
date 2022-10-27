#include <iostream>
#include <string>
#include <tuple>
#include <array>
#include <type_traits>


struct Person
{
    std::string name;
    int         age;

    /*
    ===========================================================================
    META宏展开后生成如下的成员函数，是一个tuple，其中每一项又是一个pair，包含成员变
    量的名字以及引用
    ===========================================================================
    */
    auto Meta()
    {
        return std::make_tuple(
            std::make_pair("name", std::reference_wrapper<decltype(name)>(name)),
            std::make_pair("age", std::reference_wrapper<decltype(age)>(age)));
    }
};


static void test_Kapok()
{
    // Kapok库的核心原理
    Person person;
    person.name = "Jim";
    person.age = 23;

    auto meta = person.Meta();
    auto p = std::get<0>(meta);
    p.second = "Lucy";
}


struct Person2
{
    std::string name;
    int         age;
};

// 这个数组记录每个导出的变量名
std::array<std::string, 2> arr_Person2 = { "name", "age" };


/*
===============================================================================
这个函数返回参数类型的反射信息
===============================================================================
*/
static auto iguana_reflect_members(Person2 const&)
{
    struct reflect_members
    {
        constexpr decltype(auto) static apply_impl()
        {
            // 这个tuple对应每个变量的地址
            return std::make_tuple(&Person2::name, &Person2::age);
        }

        using type = void;
        using size_type = std::integral_constant<size_t, 2>;
        static std::string name() { return "Person2"; }
        static size_t value() { return size_type::value; }
        static std::array<std::string, size_type::value> arr() { return arr_Person2; }
    };

    return reflect_members{};
}


static void test_iguana()
{
    Person2 val;
    auto meta = iguana_reflect_members(val);
    val.*std::get<0>(meta.apply_impl()) = "Jack";
    val.*std::get<1>(meta.apply_impl()) = 66;
    std::cout << val.name << std::endl << val.age;
}


int experiment_main()
{
    test_Kapok();
    test_iguana();
    return 0;
}
