#include <iostream>
#include <string>
#include <tuple>
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


int main()
{
    // Kapok库的核心原理
    Person person;
    person.name = "Jim";
    person.age = 23;

    auto meta = person.Meta();
    auto p = std::get<0>(meta);
    p.second = "Lucy";

    return 0;
}
