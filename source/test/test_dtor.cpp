/*
 * 2023/03/20:
 */

#include <iostream>
#include <assert.h>

static void some_func(const char* str)
{
	std::cout << str << std::endl;
}

int main()
{
	some_func(std::string("123").c_str()); //可以这么用。

	//下面的方式是错误的！对象析构后内存释放，野指针
#if 0
	const char* s = std::string("124").c_str();
	std::cout << s << std::endl;
#endif
	return 0;
}
