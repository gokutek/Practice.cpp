#include <iostream>

template<typename T>
void print(T const &val)
{
	std::cout << val << std::endl;
}


/*
===============================================================================
错误的写法：
void print(T const &val, TArgs... const &args)
===============================================================================
*/
template<typename T, typename... TArgs>
void print(T const &val, const TArgs&... args)
{
	std::cout << val << std::endl;
	print(args...);
}


int main()
{
	print(1);
	print(10, 20);
	print(100, 200, 300);
	return 0;
}
