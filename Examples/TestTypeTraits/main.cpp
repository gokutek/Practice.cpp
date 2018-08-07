#include <iostream>
#include <type_traits>

enum Sex
{
    BOY,
    GIRL
};


template<class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type add(T v1, T v2)
{
    std::cout << "111" << std::endl;
    return true;
}

template<class T>
typename std::enable_if<!std::is_integral<T>::value, bool>::type add(T v1, T v2)
{
    std::cout << "222" << std::endl;
    return true;
}


int main()
{
    add(1, 2);
    add(std::string("123"), std::string("123"));

    std::cout << std::boolalpha;

    std::cout << std::is_void<void>::value << std::endl; // -> true
    
    std::cout << std::is_null_pointer<std::nullptr_t>::value << std::endl; // -> true
    
    std::cout << std::is_integral<int>::value << std::endl;
    std::cout << std::is_integral<short>::value << std::endl;
    std::cout << std::is_integral<bool>::value << std::endl;

    std::cout << std::is_floating_point<float>::value << std::endl;
    std::cout << std::is_floating_point<double>::value << std::endl;

    std::cout << std::is_enum<Sex>::value << std::endl; // -> true

    std::is_polymorphic<int>::value;
    std::is_abstract<int>::value;
    std::is_empty<int>::value;
    std::is_base_of<int, long>::value;

    return 0;
}
