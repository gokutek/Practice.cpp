// 临时测试代码
#include <bitset>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>
#include <memory>
#include <initializer_list>
#include <stack>
#include <type_traits>
#include <fstream>
#include <assert.h>
#include <direct.h>
#include "reflection.h"

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


static std::string ReturnString()
{
    return "Hello";
}


static void func(int a, int b)
{
}

static void test_bitset()
{
    std::bitset<32> bs(0xfe);
    std::cout << bs.count() << std::endl;
    std::cout << bs.to_string() << std::endl;
}

class Animal 
{
public:
    virtual ~Animal() {}
};

class Cat : public Animal
{
public:
};


static void test_typeid()
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


static void MakeSureDirectoryPathExists(std::string const &dir)
{
    char *p = (char*)dir.c_str();
    while (*p)
    {
        char *x = strchr(p, '\\');
        if (!x) { break; }

        *x = 0;
        _mkdir(dir.c_str());
        p = x + 1;
        *x = '\\';
    }
}


int main()
{
    std::cout << __COUNTER__ << std::endl;
    std::cout << __COUNTER__ << std::endl;
    std::cout << __COUNTER__ << std::endl;
    std::cout << __COUNTER__ << std::endl;
    std::cout << __COUNTER__ << std::endl;
    //MakeSureDirectoryPathExists("1\\2\\3\\4\\5\\");

	extern void test_constexpr();
	extern void test_decltype();
	extern void test_macro();

	test_constexpr();
	test_decltype();
	test_macro();

    test_typeid();
    return -1;

    test_bitset();
    return -1;

    std::string xx = ReturnString();
    xx;
    //char const *str = ReturnString().c_str(); // 这种写法不安全！

	reflection::reflection_unite_test();
	test_t_type_func();
	test_t_type_func2();

	const char* sz = "hello"; // 指针指向的是常量
	sz = "world"; // 可以修改指针本身以指向其它对象
	//*sz = 'a'; // error C3892: “sz”: 不能给常量赋值

	char arr[] = "world";
	char* const sz1 = arr; // 指针本身是常量
	*sz1 = 'W'; // 可以修改指针指向的对象的值
	//sz1 = "test"; // error C3892: “sz1”: 不能给常量赋值

	char const* sz2 = "hello"; // 同sz
	sz2 = "world";
	//*sz2 = 'a';

	{
		int x = 100;
		//int & const f = x; // warning C4227: 使用了记时错误: 忽略引用上的限定符
		int & f = x;
		f = 88;
	}
	return 0;
}
