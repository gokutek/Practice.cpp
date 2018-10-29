#include <assert.h>
#include <string>
#include <iostream>
#include <memory>
#include <type_traits>
#include <initializer_list>
#include <complex>
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

/*
===============================================================================
a nontemplate function can coexist with a function template
that has the same name and can be instantiated with the same type. All other factors
being equal, the overload resolution process prefers the nontemplate over one
generated from the template.
===============================================================================
*/
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
auto maxdefault4(T1 a, T2 b)
{
	return a < b ? b : a;
}


template<typename RT, typename T1, typename T2>
RT maxdefault4(T1 a, T2 b)
{
	return a < b ? b : a;
}


TEST_CASE("max2 with RT", "basic")
{
	auto a = ::maxdefault4(4, 7.2); // uses first template
	REQUIRE(a == 7.2);

	auto b = ::maxdefault4<long double>(7.2, 4); // uses second template
	REQUIRE(b == 7.2);

	//auto c = ::maxdefault4<int>(4, 7.2); // ERROR: both function templates match
}


/*
============================================================================== =
===============================================================================
*/

// maximum of two values of any type:
template<typename T>
T max3val(T a, T b)
{
	return b < a ? a : b;
}


// maximum of two pointers:
template<typename T>
T* max3val(T* a, T* b)
{
	return *b < *a ? a : b;
}


// maximum of two C-strings:
char const* max3val(char const* a, char const* b)
{
	return std::strcmp(b, a) < 0 ? a : b;
}


TEST_CASE("max3val", "max3val")
{
	int a = 7;
	int b = 42;
	auto m1 = ::max3val(a, b); // max() for two values of type int

	std::string s1 = "hey";
	std::string s2 = "you";
	auto m2 = ::max3val(s1, s2); // max() for two values of type std::string

	int* p1 = &b;
	int* p2 = &a;
	auto m3 = ::max3val(p1, p2); // max() for two pointers

	char const* x = "hello";
	char const* y = "world";
	auto m4 = ::max3val(x, y); // max() for two C-strings
}


/*
===============================================================================
In general, it is a good idea not to change more than necessary when overloading
function templates. You should limit your changes to the number of parameters
or to specifying template parameters explicitly. Otherwise, unexpected effects
may happen. For example, if you implement your max() template to pass the
arguments by reference and overload it for two C-strings passed by value, you
can’t use the three-argument version to compute the maximum of three C-strings:
===============================================================================
*/

// maximum of two values of any type (call-by-reference)
template<typename T> 
T const& max3ref(T const& a, T const& b)
{
	return b < a ? a : b;
} 


// maximum of two C-strings (call - by - value)
char const* max3ref(char const* a, char const* b)
{
	return std::strcmp(b, a) < 0 ? a : b;
}


// maximum of three values of any type (call-by-reference)
template<typename T>
T const& max3ref(T const& a, T const& b, T const& c)
{
	return max3ref(max3ref(a, b), c); // error if max(a,b) uses	call - by - value
} 


TEST_CASE("max3ref", "max3ref")
{
	auto m1 = ::max3ref(7, 42, 68); // OK
	char const* s1 = "frederic";
	char const* s2 = "anica";
	char const* s3 = "lucas";
	//auto m2 = ::max3ref(s1, s2, s3); //run-time ERROR
}


/*
===============================================================================
In addition, ensure that all overloaded versions of a function are declared
before the function is called.
===============================================================================
*/

// maximum of two values of any type:
template<typename T>
T max4(T a, T b)
{
	return b < a ? a : b;
}


// maximum of three values of any type:
template<typename T>
T max4(T a, T b, T c)
{
	// uses the template version even for ints, because the following declaration comes too late
	return max4(max4(a, b), c);
}


// maximum of two int values:
int max4(int a,	int b)
{
	assert(0);
	return b < a ? a : b;
}


TEST_CASE("max4", "max4")
{
	::max4(47, 11, 33); // OOPS: uses max<T>() instead of max(int, int)
}

#pragma endregion


#pragma region C.2 Simplified Overload Resolution

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


/*
===============================================================================
The implicit conversion provided through the converting constructor is not
considered during template argument deduction.
===============================================================================
*/
template<typename T>
class MyString
{
public:
    MyString(T const*) 
    {
    }
};


template<typename T>
MyString<T> truncate(MyString<T> const &str, int)
{
    return str;
}


TEST_CASE("rule.test.4", "Overload Resolution")
{
    MyString<char> str1 = truncate<char>("Hello World", 5); // OK

    //MyString<char> str2 = truncate("Hello World", 5); // ERROR
}


/*
===============================================================================
an rvalue reference to a template parameter can deduce to either an lvalue
reference type (after reference collapsing) if the corresponding argument is an
lvalue or to an rvalue reference type if that argument is an rvalue
===============================================================================
*/
template<typename T>
void strange(T &&a, T &&b)
{
    a = b = 5.5;
}


template<typename T>
void bizarre(T&&, double&&)
{
}


TEST_CASE("rule.test.5", "Overload Resolution")
{
    strange(1.2, 3.4); // OK: with T deduced to double

    double val = 1.2;
    strange(val, val); // OK: with T deduced to double&
    REQUIRE(val == 5.5);

    //strange(val, 3.4); // ERROR: conflicting deductions

    //bizarre(val, val); // ERROR: lvalue val doesn’t match double&&
    bizarre(val, double(val));
}

#pragma endregion


#pragma region C.2.1 The Implied Argument for Member Functions

/*
===============================================================================
At first, nothing seems ambiguous about the expression str[5]. The subscript
operator at #1 seems like a perfect match. However, it is not quite perfect because
the argument 5 has type int, and the operator expects an unsigned integer type
(size_t and std::size_t usually have type unsigned int or unsigned
long, but never type int). Still, a simple standard integer conversion makes #1
easily viable. However, there is another viable candidate: the built-in subscript
operator. Indeed, if we apply the implicit conversion operator to str (which is the
implicit member function argument), we obtain a pointer type, and now the built-in
subscript operator applies. This built-in operator takes an argument of type
ptrdiff_t, which on many platforms is equivalent to int and therefore is a
perfect match for the argument 5. So even though the built-in subscript operator is a
poor match (by user-defined conversion) for the implied argument, it is a better
match than the operator defined at #1 for the actual subscript! Hence the potential
ambiguity.2 To solve this kind of problem portably, you can declare operator [ ]
with a ptrdiff_t parameter, or you can replace the implicit type conversion to
char* by an explicit conversion (which is usually recommended anyway).
===============================================================================
*/
class BadString
{
public:
	BadString(char const *str)
		: str_(str)
	{
	}

	char& operator[](size_t index)
	{
		return str_[index];
	}

	char const& operator[](size_t index) const
	{
		return str_[index];
	}

	operator char*()
	{
		return (char*)str_.c_str();
	}

	operator char const*() const
	{
		return str_.c_str();
	}

private:
	std::string str_;
};


TEST_CASE("rule.test.6", "Overload Resolution")
{
	BadString str("correkt");
	//str[5] = 'c'; // ERROR: possibly an overload resolution ambiguity!
}


struct S
{
	int f1()
	{
		// implicit *this parameter is an lvalue reference(see below)
		return 1;
	}

	int f2() &&
	{
		// implicit *this parameter is an rvalue reference
		return 2;
	}

	int f3() &
	{
		// implicit *this parameter is an lvalue reference
		return 3;
	}
};


TEST_CASE("rule.test.7", "Overload Resolution")
{
	S().f1(); // OK: old rule allows rvalue S() to match implied
	S().f2(); // lvalue reference type S& of *this. OK: rvalue S() matches rvalue reference type of *thisof *this
	//S().f3(); // ERROR: rvalue S() cannot match explicit lvalue reference type of *this
}

#pragma endregion


#pragma region C.2.2 Refining the Perfect Match

/*
===============
引用和const引用
===============
*/
static int report(int&)
{
	return 1;
}


static int report(int const&)
{
	return 2;
}


TEST_CASE("rule.test.8", "Overload Resolution")
{
	for (int k = 0; k < 10; ++k) {
		REQUIRE(1 == report(k)); // calls #1
	}
	REQUIRE(2 == report(42)); // calls #2
}


/*
==================
const引用和右值引用
==================
*/
struct Value
{
	Value() {}
};


static int pass(Value const&)
{
	return 1;
}

static int pass(Value &&)
{
	return 2;
}


static int g1(Value &&x)
{
	return pass(x);
}


static int g2(Value &&x)
{
	return pass(std::move(x));
}


static int g3()
{
	return pass(Value());
}


TEST_CASE("rule.test.9", "Overload Resolution")
{
	REQUIRE(1 == g1(Value())); // calls #1 , because x is an lvalue
	REQUIRE(2 == g2(Value())); // calls #2 , because X() is an rvalue (in fact, prvalue)
	REQUIRE(2 == g3()); // calls #2 , because std::move(x) is an rvalue (in fact, xvalue)
}


class Wonder
{
public:
	int tick()
	{
		return 1;
	}

	int tick() const
	{
		return 2;
	}

	int tack() const
	{
		return 3;
	}
};


TEST_CASE("rule.test.10", "Overload Resolution")
{
	Wonder wonder;
	Wonder &device = wonder;

	REQUIRE(1 == device.tick()); // calls #1
	REQUIRE(3 == device.tack()); // calls #3 , because there is no non-const version of Wonder::tack()
}


/*
===============================================================================
Finally, the following modification of our earlier example illustrates that two perfect
matches can also create an ambiguity if you overload with and without references:
===============================================================================
*/
int c2_report(int)
{
	return 1;
}


int c2_report(int&)
{
	return 2;
}


int c2_report(int const&)
{
	return 3;
}


TEST_CASE("rule.test.11", "Overload Resolution")
{
	for (int k = 0; k<10; ++k) {
		//c2_report(k); // ERROR, ambiguous: #1 and #2 match equally well
	}
	//c2_report(42); // ERROR, ambiguous: #1 and #3 match equally well
}

#pragma endregion


#pragma region C.3.1 Prefer Nontemplates or More Specialized Templates

template<typename T>
int c_3_1_f(T)
{
	return 1;
}


int c_3_1_f(int)
{
	return 2;
}


TEST_CASE("rule.test.12", "Overload Resolution")
{
	REQUIRE(2 == c_3_1_f(7));
}

#pragma endregion


#pragma region C.3.2 Conversion Sequences

/*
===============================================================================
1. A conversion of object from Derived const to Base const (this is a
glvalue conversion; it preserves the identity of the object)
2. A user-defined conversion of the resulting Base const object to type short 3.
A promotion of short to int
===============================================================================
*/

class c_3_2_Base 
{
public:
	virtual ~c_3_2_Base() {}

	operator short() const
	{
		return 1;
	}
};


class c_3_2_Derived : public c_3_2_Base 
{
};


static int c_3_2_count(int val)
{
	return val;
}


TEST_CASE("rule.test.13", "Overload Resolution")
{
	// 先转换到c_3_2_Base基类，再调用基类的转换函数转换到short，转换到int
	c_3_2_Derived object;
	REQUIRE(1 == c_3_2_count(object)); // matches with user-defined conversion
}

#pragma endregion


#pragma region C.3.3 Pointer Conversions

/*
===============================================================================
Pointers and pointers to members undergo various special standard conversions,
including
• Conversions to type bool
• Conversions from an arbitrary pointer type to void*
• Derived-to-base conversions for pointers
• Base-to-derived conversions for pointers to members
===============================================================================
*/


/*
===============================================================================
conversions to type bool (both from a regular pointer and from a pointer to
a member) are considered worse than any other kind of standard conversion.
===============================================================================
*/
int c_3_3_check(void*)
{
	return 1;
}


int c_3_3_check(bool)
{
	return 2;
}


TEST_CASE("rule.test.14", "Overload Resolution")
{
	std::unique_ptr<int> p(new int(10));
	REQUIRE(c_3_3_check(p.get()) == 1);
}


/*
===============================================================================
if conversions to different classes related by inheritance exist,
a conversion to the most derived class is preferred.
===============================================================================
*/
class c_3_3_interface
{
public:
	virtual ~c_3_3_interface() {}
};


class c_3_3_CommonProcesses : public c_3_3_interface
{
};


class c_3_3_Machine : public c_3_3_CommonProcesses
{
};


int c_3_3_serialize(c_3_3_interface*)
{
	return 1;
}


int c_3_3_serialize(c_3_3_CommonProcesses*)
{
	return 2;
}


TEST_CASE("rule.test.15", "Overload Resolution")
{
	c_3_3_Machine *machine = NULL;
	REQUIRE(c_3_3_serialize(machine) == 2);
}

#pragma endregion


#pragma region C.3.4 Initializer Lists

/*
===============================================================================
Initializer list arguments (initializers passed with in curly braces) can be converted to
several different kinds of parameters: initializer_lists, class types
with an initializer_list constructor, class types for which the initializer
list elements can be treated as (separate) parameters to a constructor, or aggregate
class types whose members can be initialized by the elements of the initializer list.
===============================================================================
*/
int c_3_4_f(std::initializer_list<int>)
{
	return 1;
}


int c_3_4_f(std::initializer_list<std::string>)
{
	return 2;
}


int c_3_4_g(std::vector<int> const& vec)
{
	return 3;
} 


int c_3_4_h(std::complex<double> const& cmplx)
{
	return 4;
}


struct c_3_4_Point
{
	int x, y;
};


int c_3_4_i(c_3_4_Point const &pt)
{
	return 5;
} 


TEST_CASE("rule.test.16", "Overload Resolution")
{
	REQUIRE(1 == c_3_4_f({ 1, 2, 3 }));
	REQUIRE(2 == c_3_4_f({ "hello", "initializer", "list" }));
	REQUIRE(3 == c_3_4_g({ 1, 1, 2, 3, 5 }));
	REQUIRE(4 == c_3_4_h({ 1.5, 2.5 }));
	REQUIRE(5 == c_3_4_i({ 1, 2 }));
}


/*
===============================================================================
===============================================================================
*/
static int c_3_4_ovl(std::initializer_list<char>) 
{
	return 1;
}


static int c_3_4_ovl(std::initializer_list<int>)
{
	return 2;
}


TEST_CASE("rule.test.17", "Overload Resolution")
{
	REQUIRE(1 == c_3_4_ovl({ 'h', 'e', 'l', 'l', 'o', '\0' }));
	REQUIRE(2 == c_3_4_ovl({ 'h', 'e', 'l', 'l', 'o', 0 }));
}


/*
===============================================================================
any initializer-list constructor is a better match than any
non-initializer-list constructor.
===============================================================================
*/
template<typename T>
struct C_3_4_Array
{
	C_3_4_Array(std::initializer_list<T>)
	{
		ver = 1;
	}

	C_3_4_Array(unsigned n, T const&)
	{
		ver = 2;
	}

	int ver;
};


int C_3_4_arr1(C_3_4_Array<int> arr)
{
	return arr.ver;
}


int C_3_4_arr2(C_3_4_Array<std::string> arr)
{
	return arr.ver;
}


TEST_CASE("rule.test.18", "Overload Resolution")
{
	REQUIRE(1 == C_3_4_arr1({ 1, 2, 3, 4, 5 }));
	REQUIRE(1 == C_3_4_arr1({ 1, 2 }));
	REQUIRE(1 == C_3_4_arr1({ 10u, 5 }));
	REQUIRE(1 == C_3_4_arr2({ "hello", "initializer", "list" }));
	REQUIRE(2 == C_3_4_arr2({ 10, "hello" }));
}

#pragma endregion


#pragma region C.3.5 Functors and Surrogate Functions

/*
===============================================================================
The surrogate function has a worse match for the implied
parameter (because it requires a user-defined conversion), but it has a better match
for the last parameter; hence the two candidates cannot be ordered. The call is
therefore ambiguous.
===============================================================================
*/
using FuncType = void(double, int);


class IndirectFunctor 
{
public:
	void operator()(double, double) const
	{
	}
	
	// Surrogate functions are in the most obscure corners of C++ and rarely occur in practice(fortunately).
	operator FuncType*() const
	{
		return NULL;
	}
};


TEST_CASE("rule.test.19", "Overload Resolution")
{
	IndirectFunctor const *p = NULL;
	IndirectFunctor const& funcObj = *p;
	// ERROR: ambiguous，可能是调用operator()操作符，也可能是先转换成函数指针再调用
	//funcObj(3, 5);
}

#pragma endregion


#pragma region C.3.6 Other Overloading Contexts

int c_3_6_numElems(int)
{
	return 1;
}


int c_3_6_numElems(char)
{
	return 2;
}


TEST_CASE("rule.test.20", "Overload Resolution")
{
	int(*funcPtr)(char) = c_3_6_numElems;
	REQUIRE(funcPtr(1) == 2);
}


class c_3_6_BigNum
{
public:
	c_3_6_BigNum(long n) 
	{
		ver = 1;
	}

	c_3_6_BigNum(double n) 
	{
		ver = 2;
	}

	c_3_6_BigNum(std::string const&) 
	{
		ver = 3;
	}
	
	operator double() 
	{
		return 4;
	}

	operator long() 
	{
		return 5;
	}

	int ver;
};


TEST_CASE("rule.test.21", "Overload Resolution")
{
	// 注意，下面一行代码无法编译，可能是VS的编译器不支持
	//c_3_6_BigNum bn1(100103);
	//REQUIRE(bn1.ver == 1);

	c_3_6_BigNum bn2("7057103224.095764");
	REQUIRE(bn2.ver == 3);

	int in = bn2;
	REQUIRE(in == 5);
}

#pragma endregion
