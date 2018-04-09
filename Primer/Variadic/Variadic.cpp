// 16.4
#include <string>
#include <iostream>

template<typename T, typename... Args>
void foo(const T& t, const Args&... rest)
{
	/*
	==============================================================================
	什么情况下sizeof...(Args)与sizeof...(rest)才不相等呢？
	我以为对foo(0, 1, 2, 3, 4, 5)来说，sizeof...(Args)应该返回1，因为参数类型都是int
	==============================================================================
	*/
	std::cout << "sizeof...(Args) = " << sizeof...(Args) << std::endl;
	std::cout << "sizeof...(rest) = " << sizeof...(rest) << std::endl;
}


// 理解为“递归终止函数”
template<typename T>
std::ostream& print(std::ostream& os, const T& t)
{
	return os << t << std::endl;
}


template<typename T, typename... Args>
std::ostream& print(std::ostream& os, const T& t, const Args&... rest)
{
	os << t << ", ";
	// 递归调用
	return print(os, rest...);
}


int main()
{
	int i = 0;
	double d = 3.14;
	std::string s = "how now brown cow";
	foo(i, s, 42, d);
	foo(s, 32, "hi");
	foo(d, s);
	foo("hi");
	foo(0, 1, 2, 3, 4, 5);
	print(std::cout, 1);
	print(std::cout, 1, 2);
	print(std::cout, 1, 2, 3);
	print(std::cout, 1, 2, 3, 4);
	print(std::cout, 1, 2, 3, 4, 5);
	return 0;
}
