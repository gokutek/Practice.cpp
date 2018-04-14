#ifndef APPLY1_HPP
#define APPLY1_HPP

#include <iostream>

template<typename T>
inline void apply(T& arg, void(*func)(T))
{
	func(arg);
}


inline void incr(int& a)
{
	++a;
}


inline void print(int a)
{
	std::cout << a << std::endl;
}


inline void UnitTest_Apply1()
{
	std::cout << "TEST: " << __FUNCTION__ << std::endl;
	int x = 7;
	apply(x, print); // T������Ϊint

	/*
	===
	�����ϲ�һ������ǿ�Ƽ�<int&>���벻��ȥ��
	��ʱTΪint&������һ������������int&& arg������int& arg
	===
	*/
	apply<int&>(x, incr);
	std::cout << x << std::endl;
}

#endif // APPLY1_HPP
