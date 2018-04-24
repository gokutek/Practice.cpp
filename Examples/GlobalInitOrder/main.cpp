/*
===============================================================================
POD类型的全局变量先初始化
===============================================================================
*/
#include <iostream>

struct MyStruct
{
public:
	MyStruct(int x)
		: val(x)
	{
		static int count = 1;
		std::cout << "count = " << count << std::endl;
		++count;

		extern int g_IntVal;
		std::cout << "g_IntVal = " << g_IntVal << std::endl;
		++g_IntVal;
	}

	int val;
};

MyStruct	g_MyStuct1(1);
int g_IntVal = 888;
MyStruct	g_MyStuct2(2);


int main()
{
	std::cout << "g_IntVal = " << g_IntVal << std::endl;
	return 0;
}
