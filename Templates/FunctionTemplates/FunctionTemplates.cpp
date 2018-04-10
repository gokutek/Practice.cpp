#include <iostream>

template<typename T>
T const& _max(T const& a, T const& b)
{
	return a > b ? a : b;
}


// 返回值类型不能使用：T1 const&，返回局部对象的引用
template<typename T1, typename T2>
T1 _max2(T1 const& a, T2 const& b)
{
#pragma warning(push)
#pragma warning(disable:4244)
	return a > b ? a : b;
#pragma warning(pop)
}


template<typename T1, typename T2, typename RT>
RT _max3(T1 const& a, T2 const& b)
{
#pragma warning(push)
#pragma warning(disable:4244)
	return a > b ? a : b;
#pragma warning(pop)
}


template<typename RT, typename T1, typename T2>
RT _max4(T1 const& a, T2 const& b)
{
#pragma warning(push)
#pragma warning(disable:4244)
	return a > b ? a : b;
#pragma warning(pop)
}


template<typename T>
T const& _max5(T const& a, T const& b)
{
	return a > b ? a : b;
}


char const* _max5(char const* a, char const* b)
{
	return strcmp(a, b) < 0 ? b : a;
}


template<typename T>
T const& _max5(T const& a, T const& b, T const& c)
{
	return _max5(_max5(a, b), c);
}


int main()
{
	std::cout << _max(1, 2) << std::endl;
	std::cout << _max(1.2, 2.3) << std::endl;
	std::cout << _max(1.21f, 2.34f) << std::endl;

	//std::cout << _max(5.6, 12) << std::endl; // 无法编译，两个参数的类型不同
	std::cout << _max(5.6, static_cast<double>(12)) << std::endl; // 强制将第二个参数转为double
	std::cout << _max<double>(5.6, 12) << std::endl; // 显式指定模板参数

	std::cout << _max2(2.5, 10) << std::endl;
	std::cout << _max2(10, 2.5) << std::endl;

	std::cout << _max3<int, double, double>(110, 112.5) << std::endl;

	std::cout << _max4<double>(1110, 1112.5) << std::endl;

	std::cout << _max5(7, 42, 68) << std::endl;
	const char* s1 = "s1";
	const char* s2 = "s2";
	const char* s3 = "s3";
	//std::cout << _max5(s1, s2, s3) << std::endl; // C4172: 返回局部变量或临时变量的地址

	return 0;
}
