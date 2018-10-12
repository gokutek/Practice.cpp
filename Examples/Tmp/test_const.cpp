#include "catch.hpp"


TEST_CASE("const", "[const]")
{
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
}
