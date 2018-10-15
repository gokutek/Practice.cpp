#include <iostream>
#include "catch.hpp"


/*
===================================
typeid，在实际项目中未发现有什么应用
===================================
*/


namespace
{
    class Animal
    {
    public:
        virtual ~Animal() {}
    };


    class Cat : public Animal
    {
    public:
    };
}


#define LOG_OUT 0


TEST_CASE("typeid", "[typeid]")
{
    {
        int x = 0;
        const type_info& info = typeid(&x);
#if LOG_OUT
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
#endif
    }

    {
#if LOG_OUT
        std::cout << std::endl;
#endif
        std::vector<int> vec;
        const type_info& info = typeid(&vec);
#if LOG_OUT
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
#endif
    }

    {
#if LOG_OUT
        std::cout << std::endl << "Cat cat;" << std::endl;
        std::cout << "typeid(&cat);" << std::endl;
#endif
        Cat cat;
        const type_info& info = typeid(&cat);
#if LOG_OUT
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
#endif
    }

    {
#if LOG_OUT
        std::cout << std::endl << "Cat cat;" << std::endl;
        std::cout << "typeid(cat);" << std::endl;
#endif
        Cat cat;
        const type_info& info = typeid(cat);
#if LOG_OUT
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
#endif
    }

    {
#if LOG_OUT
        std::cout << std::endl << "Cat cat;" << std::endl;
        std::cout << "Animal &animal = cat;" << std::endl;
        std::cout << "typeid(animal);" << std::endl;
#endif
        Cat cat;
        Animal &animal = cat;
        const type_info& info = typeid(animal);
#if LOG_OUT
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
#endif
    }

    {
#if LOG_OUT
        std::cout << std::endl << "Cat cat;" << std::endl;
        std::cout << "Animal *animal = &cat;" << std::endl;
        std::cout << "typeid(animal);" << std::endl;
#endif
        Cat cat;
        Animal *animal = &cat;
        const type_info& info = typeid(animal);
#if LOG_OUT
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
#endif
    }

    {
#if LOG_OUT
        std::cout << std::endl << "typeid(Animal);" << std::endl;
#endif
        const type_info& info = typeid(Animal);
#if LOG_OUT
        std::cout << "name: " << info.name() << std::endl;
        std::cout << "hash_code: " << info.hash_code() << std::endl;
        std::cout << "raw_name: " << info.raw_name() << std::endl;
#endif
    }
}
