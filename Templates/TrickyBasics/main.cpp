#include <iostream>

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


int main()
{
	Shell<int>::In<5>::Deep<5> deep;
	Weird<int, 5> w;
	w.case1(&deep);

	Derived<int> bar;
	bar.foo();

	return 0;
}
