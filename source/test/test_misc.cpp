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

int main()
{
	//test_nothrow();
	test_pure_dtor();
	test_2d_array();
	return 0;
}
