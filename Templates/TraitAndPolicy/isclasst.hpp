#ifndef ISCLASST_HPP
#define ISCLASST_HPP

template<typename T>
class IsClassT
{
	// 下面两个test函数只有声明没有定义，运行时并没有地方调用它们。
	template<typename C>
	static char test(int C::*);

	template<typename C>
	static int test(...);

public:
	enum { Yes = sizeof(test<T>(0)) == 1 };
};

/*
===============================================================================
上面test函数参数中的int C::*，表示一个int指针类型，指向C的一个int成员变量。

class AAA
{
public:
	int64_t xxx;
	int X;
};

int (AAA::*p) = &AAA::X; // p = 8，X的偏移值
===============================================================================
*/

#endif // ISCLASST_HPP
