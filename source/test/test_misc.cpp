/*
 * 2023/03/20: 对之前的代码整理，去除catch2单元测试库的依赖
 */

#include <assert.h>
#include <iostream>

/*
=========================
一些不常用的C++特性、关键字
=========================
*/

/*
=========================================================
加上“(std::nothrow)”，内存分配失败时返回NULL，而不是抛出异常
=========================================================
*/
static void test_nothrow()
{
#ifdef WIN32
	char* p = new (std::nothrow) char[0x7ffffff];
    assert(p == NULL);
#endif // WIN32
}

/*
===============================================================================
将析构函数声明为纯虚函数仅在一种情况下有用：这个类没有其它纯虚接口。
此时，将析构函数声明为纯虚函数，可以阻止该类型的实例化。
===============================================================================
*/
namespace
{
	class Animal
	{
	public:
		virtual ~Animal() = 0;
	};


	// 必须要实现，否则将会有链接错误
	inline Animal::~Animal()
	{
	}

	class Cat : public Animal
	{
	public:
	};
}

static void test_pure_dtor()
{
	//Animal ani;
	Cat cat;
	cat;
}

static void test_2d_array()
{
	/*
	==========================================
	二维数组在内存中，先存第一行，再存第二行...，
	因此也叫行主序。
	==========================================
	*/
	int matrix[4][4] = {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
	};

	assert(matrix[2][3] == 12);

	int *ptr = &matrix[0][0];
	assert(ptr[5] == 6);
}

static void test_my_assert()
{
	/*
	==========================================
	由于RELEASE_MYASSERT1宏展开后时空的，所以函数
	也不会被调用。
	==========================================
	*/

	auto f = [](int a, int b) {
		int sum = a + b;
		std::cout << "sum=" << sum << std::endl;
		return sum;
	};

#define DEBUG_MYASSERT(cond) if (!cond) { abort(); }
#define RELEASE_MYASSERT1(cond)
#define RELEASE_MYASSERT2(cond) cond

	DEBUG_MYASSERT(f(1, 1));
	RELEASE_MYASSERT1(f(1, 2));
	RELEASE_MYASSERT2(f(1, 3));
}

static void test_order()
{
	//乘法的优先级高于异或
	int v1 = 10 * 2 ^ 1;
	int v3 = (10 * 2) ^ 1;

	int v2 = 10 * (2 ^ 1);
	assert(v1 == v3);
	assert(v1 != v2);
}

static void test_hash()
{
	std::hash<uint32_t> func;
	uint32_t i = 123;
	std::cout << func(i) << std::endl;
}

int main()
{
	//test_nothrow();
	test_pure_dtor();
	test_2d_array();
	test_my_assert();
	test_order();
	test_hash();
	return 0;
}
