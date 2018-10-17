#include <iostream>
#include <type_traits>
#include "catch.hpp"


#pragma region 1.1.1 Defining the Template

/*
===============================================================================
需要思考的问题：
1.参数是否会拷贝？
===============================================================================
*/
template<typename T>
T max1(T a, T b)
{
	return b < a ? a : b;
}


TEST_CASE("max1", "basic")
{
	REQUIRE(max1(1, 2) == 2);
	REQUIRE(max1(10.0, 20.0) == 20.0);
	REQUIRE(max1(std::string("Hello"), std::string("world")) == "world");
}

#pragma endregion


#pragma region 1.1.2 Using the Template

TEST_CASE("Using the Template", "basic")
{
	// int max (int, int);
	int i = 42;
	REQUIRE(42 == ::max1(7, i));

	// double max (double, double);
	double f1 = 3.4; 
	double f2 = -6.7;
	REQUIRE(::max1(f1, f2) == 3.4);

	// std::string max (std::string, std::string);
	std::string s1 = "mathematics"; 
	std::string s2 = "math";
	REQUIRE(::max1(s1, s2) == s1);
}

#pragma endregion


#pragma region 1.2 Template Argument Deduction

template<typename T>
T max_1_2(T const& a, T const& b)
{
	return b < a ? a : b;
}


template<typename T>
T foo(T*, T*)
{
	return T();
}


TEST_CASE("Type Conversions During Type Deduction", "basic")
{
	int i = 21;
	int const c = 42;
	max1(i, c); // OK: T is deduced as int
	max1(c, c); // OK: T is deduced as int
	int& ir = i;
	max1(i, ir); // OK: T is deduced as int
	int arr[4];
	foo(&i, arr); // OK: T is deduced as int*

	// foo("hello", std::string()); // compile ERROR: T can be deduced as char const[6] or std::string
	// max1(4, 7.2); // compile ERROR: T can be deduced as int or double
	
	REQUIRE(max1(static_cast<double>(4), 7.2) == 7.2);
	REQUIRE(max1<double>(4, 7.2) == 7.2);
}


template<typename T>
void f(T = "")
{
}


TEST_CASE("Type Deduction for Default Arguments", "basic")
{
	// 这也可以，调用f<int>。但是直接定义“void f(int = "")”这样的函数是编译不过的
	f(0);
}

#pragma endregion


#pragma region 1.3.1 Template Parameters for Return Types

/*
===============================================================================
将返回值类型作为模板参数，这个模板参数不能从调用参数中推导，必须手动指定。同时，在这
个实现中，用户还必须指定前面的两个模板参数
===============================================================================
*/
template<typename T1, typename T2, typename RT>
RT _max3(T1 a, T2 b)
{
	return a < b ? b : a;
}


TEST_CASE("_max3", "basic")
{
	float f = _max3<int, float, float>(1, 2.0f);
	REQUIRE(f == 2.0f);
}


/*
===============================================================================
改进max3，将RT模板参数移到最前面，这样用户使用的时候只需要指定这一个模板参数就好了
===============================================================================
*/
template<typename RT, typename T1, typename T2>
RT _max4(T1 a, T2 b)
{
	return a < b ? b : a;
}


TEST_CASE("_max4", "basic")
{
	float f = _max4<float>(1, 2.0f);
	REQUIRE(f == 2.0f);
}

#pragma endregion


#pragma region 1.3.2 Deducing the Return Type

/*
===============================================================================
继续改进max4，C++14开始，比较正统的做法。
In fact, the use of auto for the return type without a corresponding trailing return
type (which would be introduced with a -> at the end) indicates that the actual return
type must be deduced from the return statements in the function body. Of course,
deducing the return type from the function body has to be possible. Therefore, the
code must be available and multiple return statements have to match.
===============================================================================
*/
template<typename T1, typename T2>
auto maxauto(T1 a, T2 b)
{
	return a < b ? b : a;
}


/*
===============================================================================
调用下面的代码将会产生编译错误：
error C3487: “float”: 所有返回表达式必须推导为相同类型: 以前为“int”。
因为函数中有两个return语句，并且它们的返回类型不一样。
===============================================================================
*/
template<typename T1, typename T2>
auto maxauto_compile_error(T1 a, T2 b)
{
	if (1) {
		return 1;
	}
	return 2.0f;
}


TEST_CASE("maxauto", "basic")
{
	float f = maxauto(1, 2.0f);
	REQUIRE(f == 2.0f);
}


/*
===============================================================================
使用decltype来定义返回类型（不需要写成decltype(b<a?a:b)）
===============================================================================
*/
template<typename T1, typename T2>
auto maxdecltype(T1 a, T2 b) -> decltype(true ? a : b)
{
	return a < b ? b : a;
}


TEST_CASE("maxdecltype", "basic")
{
	float f = maxdecltype(1, 2.0f);
	REQUIRE(f == 2.0f);
}


/*
===============================================================================
改进max6：
It might happen that the return type is a reference type, because under some
conditions T might be a reference. For this reason you should return the type
decayed from T.
===============================================================================
*/
template<typename T1, typename T2>
auto maxdecltypedecay(T1 a, T2 b) -> typename std::decay<decltype(true ? a : b)>::type
{
	return a < b ? b : a;
}


TEST_CASE("maxdecltypedecay", "basic")
{
	float f = maxdecltypedecay(1, 2.0f);
	REQUIRE(f == 2.0f);
}


/*
===============================================================================
an initialization of type auto always decays
===============================================================================
*/
TEST_CASE("auto dacay", "basic")
{
	int i = 42;
	int const &ir = i;
	auto a = ir;
	a = 15;
	REQUIRE(i == 42);
	REQUIRE(a == 15);
}

#pragma endregion


#pragma region 1.3.3 Return Type as Common Type

/*
===============================================================================
使用C++11的std::common_type来定义返回值类型。
C++14中，可以简化写成：std::common_type_t<T1, T2>。
===============================================================================
*/
template<typename T1, typename T2>
typename std::common_type<T1, T2>::type maxcommon(T1 a, T2 b)
{
	return a < b ? b : a;
}


TEST_CASE("maxcommon", "basic")
{
	float f = maxcommon(1, 2.0f);
	REQUIRE(f == 2.0f);
}

#pragma endregion


#pragma region 1.4 Default Template Arguments

/*
===============================================================================
演示Default Template Arguments的用法
===============================================================================
*/
template<typename T1, typename T2, typename RT = std::decay_t<decltype(true?T1():T2())>>
RT maxdefault1(T1 a, T2 b)
{
	return a < b ? b : a;
}


TEST_CASE("maxdefault1", "basic")
{
	float f = maxdefault1(1, 2.0f);
	REQUIRE(f == 2.0f);

	double d = maxdefault1<int, float, double>(1, 2.0f);
	REQUIRE(d == 2.0);
}


/*
===============================================================================
改进maxdefault1中的RT类型定义，使用std::common_type_t
===============================================================================
*/
template<typename T1, typename T2, typename RT = std::common_type_t<T1, T2>>
RT maxdefault3(T1 a, T2 b)
{
	return a < b ? b : a;
}


TEST_CASE("maxdefault3", "basic")
{
	float f = maxdefault3(1, 2.0f);
	REQUIRE(f == 2.0f);

	double d = maxdefault3<int, float, double>(1, 2.0f);
	REQUIRE(d == 2.0);
}

#pragma endregion


#pragma region 1.5 Overloading Function Templates

int max2(int a, int b)
{
	return a < b ? b : a;
}


template<typename T>
T max2(T a, T b)
{
	return a < b ? b : a;
}


TEST_CASE("max2", "basic")
{
	REQUIRE(max2(7, 42) == 42); // calls the nontemplate for two ints

	// the template is a better match because no conversion from double or char to int is required
	REQUIRE(max2(7.0, 42.0) == 42.0); // calls max<double> (by argument deduction)
	REQUIRE(max2('a', 'b') == 'b'); //calls max<char> (by argument deduction)

	// indicates that only templates may resolve a call
	REQUIRE(max2<>(7, 42) == 42); // calls max<int> (by argument deduction)

	// only the nontemplate function allows nontrivial conversions
	REQUIRE(max2<double>(7, 42) == 42.0); // calls max<double> (no argument deduction)

#pragma warning(push)
#pragma warning(disable: 4244)

	REQUIRE(max2('a', 42.7) == 'a'); //calls the nontemplate for two ints

#pragma warning(pop)
}


template<typename T1, typename T2>
auto max2(T1 a, T2 b)
{
	return a < b ? b : a;
}


template<typename RT, typename T1, typename T2>
RT max2(T1 a, T2 b)
{
	return a < b ? b : a;
}


TEST_CASE("max2 with RT", "basic")
{
	auto a = ::max2(4, 7.2); // uses first template
	REQUIRE(a == 7.2);

	auto b = ::max2<long double>(7.2, 4); // uses second template
	REQUIRE(b == 7.2);

	//auto c = ::max2<int>(4, 7.2); // ERROR: both function templates match
}

#pragma endregion


#pragma region Appendix C Overload Resolution

/*
===============================================================================
• The name is looked up to form an initial overload set.
• If necessary, this set is adjusted in various ways (e.g., template argument deduction
and substitution occurs, which can cause some function template candidates to be
discarded).
• Any candidate that doesn’t match the call at all (even after considering implicit
conversions and default arguments) is eliminated from the overload set. This
results in a set of viable function candidates.
• Overload resolution is performed to find a best candidate. If there is one, it is
selected; otherwise, the call is ambiguous.
• The selected candidate is checked. For example, if it is a deleted function (i.e., one
defined with = delete) or an inaccessible private member function, a diagnostic
is issued.
===============================================================================
*/


/*
===============================================================================
1. Perfect match. The parameter has the type of the expression, or it has a type that is
a reference to the type of the expression (possibly with added const and/or
volatile qualifiers).
2. Match with minor adjustments. This includes, for example, the decay of an array
variable to a pointer to its first element or the addition of const to match an
argument of type int** to a parameter of type int const* const*.
3. Match with promotion. Promotion is a kind of implicit conversion that includes
the conversion of small integral types (such as bool, char, short, and
sometimes enumerations) to int, unsigned int, long, or unsigned
long, and the conversion of float to double.
4. Match with standard conversions only. This includes any sort of standard
conversion (such as int to float) or conversion from a derived class to one of
its public, unambiguous base classes but excludes the implicit call to a conversion
operator or a converting constructor.
5. Match with user-defined conversions. This allows any kind of implicit conversion.
6. Match with ellipsis (…). An ellipsis parameter can match almost any type.
However, there is one exception: Class types with a nontrivial copy constructor
may or may not be valid (implementations are free to allow or disallow this).
===============================================================================
*/


int append_c_f1(int)
{
	return 1;
}


int append_c_f1(double)
{
	return 2;
}


TEST_CASE("rule.test.1", "Overload Resolution")
{
	REQUIRE(append_c_f1(4) == 1); // calls #1 : perfect match (#2 requires a standard conversion)
}


int append_c_f2(int)
{
	return 3;
}


int append_c_f2(char)
{
	return 4;
}


TEST_CASE("rule.test.2", "Overload Resolution")
{
	// calls #3 : match with promotion (#4 requires stronger standard conversion)
	REQUIRE(append_c_f2(true) == 3);
}


class X
{
public:
	X(int) {}
};


int append_c_f3(X)
{
	return 5;
}


int append_c_f3(...)
{
	return 6;
}


TEST_CASE("rule.test.3", "Overload Resolution")
{
	// calls #5 : match with user-defined conversion (#6 requires a match with ellipsis)
	REQUIRE(append_c_f3(7) == 5);
}

#pragma endregion
