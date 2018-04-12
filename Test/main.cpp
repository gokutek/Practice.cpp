// 临时测试代码
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


int main()
{
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
	return 0;
}
