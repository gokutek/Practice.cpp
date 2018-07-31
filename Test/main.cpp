﻿// 临时测试代码
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
#include "reflection.h"
#include "test_decltype.h"
#include "test_constexpr.h"


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


int main()
{
    test_bitset();
    return -1;

    std::string xx = ReturnString();
    xx;
    //char const *str = ReturnString().c_str(); // 这种写法不安全！

    test_constexpr();
    test_decltype();

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
