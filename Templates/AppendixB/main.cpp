#include <iostream>

void combine(int, double)
{
}


void combine(long, int)
{
}


void f1(int)
{
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}


void f1(double)
{
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}


void f2(int)
{
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}


void f2(char)
{
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}


class X
{
public:
	X(int)
	{
	}
};


void f3(X)
{
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}


void f3(...)
{
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}


template<typename T>
class MyString
{
public:
	MyString(T const*)
	{
	}
};


template<typename T>
void truncate(MyString<T> const&, int)
{
}


class BadString
{
public:
	BadString(char const*)
	{

	}

	char& operator[](size_t index)
	{
		return str_[index];
	}

	char const & operator[](size_t index) const
	{
		return str_[index];
	}

	operator char*()
	{
		return (char*)str_.c_str();
	}

	operator char const*()
	{
		return str_.c_str();
	}

private:
	std::string str_;
};

int main()
{
	//combine(1, 2); // 二义性
	f1(4);
	f2(true);
	f3(7); // int将会作为X的构造函数参数，构造一个X对象（如果把构造函数声明为explicit那么又是另外一个结果）
	
	truncate<char>("HelloWorld", 5);
	//truncate("HelloWorld", 5); // 编译错误，在模板实参的演绎过程中，并不会考虑这种由单参数构造函数所提供的隐式转型

	BadString str("correkt");
	//str[5] = 'c'; // 编译错误：上面的4个函数可能都匹配
	return 0;
}
