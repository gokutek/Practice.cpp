#include <iostream>
#include "stack.h"

template<typename T>
class Shell
{
public:
	template<int N>
	class In
	{
	public:
		template<int M>
		class Deep
		{
		public:
			void f()
			{
				std::cout << __FUNCTION__ << ": M = " << M << std::endl;
			}
		};
	};
};


template<typename T, int N>
class Weird
{
public:
	//void case1(Shell<T>::In<N>::Deep<N>* p) // 错误
	//void case1(typename Shell<T>::In<N>::Deep<N>* p) // 错误
	void case1(typename Shell<T>::template In<N>::template Deep<N>* p)
	{
		//p->template Deep<N>::f(); // 书上是这种写法，然后现在编译不过了。
		p->f();
		p->Deep<N>::f();
	}
};


template<typename T>
class Base 
{
public:
	void bar()
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};


template<typename T>
class Derived : Base<T> {
public:
	void foo() 
	{
		bar(); // 调用的还是基类的，没出现书上的情况，难道是VS编译器的问题吗？
		this->bar();
	}
};


static void test_stack()
{
	Stack<int> s;
	s.push(1);
	s.push(2);
	s.push(3);
	int val = s.top();
	s.pop();

	Stack<int> s2;
	s2 = s; // 这个调用不会走模板版本的operator=

	Stack<float> sFloat;
	sFloat.push(10.0f);
	sFloat.push(21.2f);
	sFloat.push(334.33f);
	s = sFloat;
}


int main()
{
	Shell<int>::In<5>::Deep<5> deep;
	Weird<int, 5> w;
	w.case1(&deep);

	Derived<int> bar;
	bar.foo();

	test_stack();
	
	return 0;
}
