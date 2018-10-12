#include <iostream>
#include "catch.hpp"


class Animal
{
public:
    virtual ~Animal() {}
};


class Cat : public Animal
{
public:
};


TEST_CASE("typeid", "[typeid]")
{
    {
        int x = 0;
        const type_info& info = typeid(&x);
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
    }

    {
        std::cout << std::endl;
        std::vector<int> vec;
        const type_info& info = typeid(&vec);
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
    }

    {
        std::cout << std::endl << "Cat cat;" << std::endl;
        std::cout << "typeid(&cat);" << std::endl;
        Cat cat;
        const type_info& info = typeid(&cat);
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
    }

    {
        std::cout << std::endl << "Cat cat;" << std::endl;
        std::cout << "typeid(cat);" << std::endl;
        Cat cat;
        const type_info& info = typeid(cat);
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
    }

    {
        std::cout << std::endl << "Cat cat;" << std::endl;
        std::cout << "Animal &animal = cat;" << std::endl;
        std::cout << "typeid(animal);" << std::endl;
        Cat cat;
        Animal &animal = cat;
        const type_info& info = typeid(animal);
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
    }

    {
        std::cout << std::endl << "Cat cat;" << std::endl;
        std::cout << "Animal *animal = &cat;" << std::endl;
        std::cout << "typeid(animal);" << std::endl;
        Cat cat;
        Animal *animal = &cat;
        const type_info& info = typeid(animal);
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
    }

    {
        std::cout << std::endl << "typeid(Animal);" << std::endl;
        const type_info& info = typeid(Animal);
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
    }
}
