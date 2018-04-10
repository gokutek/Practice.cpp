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
#include <fstream>
#include <assert.h>


template<typename T>
void test_ref(T val)
{
	val = 10;
}


void test_ref_()
{
	int a = 100;
	int& ra = a;
	test_ref(ra);
	assert(ra == 100);
}


int main()
{
	const char* sz = "hello"; // 指针指向的是常量
	sz = "world"; // 可以修改指针本身以指向其它对象
	//*sz = 'a'; // error C3892: “sz”: 不能给常量赋值

	char* const sz1 = "world"; // 指针本身是常量
	*sz1 = 'W'; // 可以修改指针指向的对象的值
	//sz1 = "test"; // error C3892: “sz1”: 不能给常量赋值

	char const* sz2 = "hello"; // 同sz
	sz2 = "world";
	//*sz2 = 'a';
	return 0;
}
