#include <assert.h>
#include "catch.hpp"
#include "var_list.h"


/*
=======
T与引用
=======
*/
template<typename T>
void test_t_type(T val)
{
    val = 55555;
}


void test_t_type_func()
{
    int a = 100;
    int &ra = a;

    // 虽然ra是int&，在到test_t_type中，T被推断为int类型
    test_t_type(ra);
    assert(a == 100);

    //强制将T设为int&
    test_t_type<int&>(ra);
    assert(a == 55555);
}


/*
=========
T&&与引用
=========
*/
template<typename T>
void test_t_type2(T&& val)
{
    val = 666666;
}


TEST_CASE("test_t_type2", "[template]")
{
	int a = 100;
	int &ra = a;
	test_t_type2(ra);
	REQUIRE(a == 666666);
}


/*
===============================================================================
将可变参数列表中的参数全部打包进VarList中
===============================================================================
*/
static void MakeVarList(VarList &varlist)
{
}


template<typename T, typename ... TArgs>
void MakeVarList(VarList &varlist, T &&val, TArgs&& ... args)
{
	varlist << val;
	MakeVarList(varlist, args...);
}


template<typename ... TArgs>
int Invoke(void *pEntity, char const *func, TArgs && ... args)
{
	VarList varlist;
	MakeVarList(varlist, args...);

	if (!strcmp(func, "func0")) {
		assert(varlist.GetCount() == 0);
		return 0;
	}

	if (!strcmp(func, "func1")) {
		assert(varlist.GetCount() == 1);
		assert(varlist.BoolVar(0) == true);
		return 1;
	}

	if (!strcmp(func, "func2")) {
		assert(varlist.GetCount() == 2);
		assert(varlist.IntVar(0) == 3);
		assert(varlist.IntVar(1) == 4);
		return 2;
	}

	if (!strcmp(func, "func3")) {
		assert(varlist.GetCount() == 3);
		assert(varlist.IntVar(0) == 3);
		assert(varlist.IntVar(1) == 4);
		assert(varlist.StringVar(2) == std::string("hello"));
		return 3;
	}

	assert(0);
	return -1;
}


TEST_CASE("Invoke", "[template]")
{
	REQUIRE(0 == Invoke(NULL, "func0"));
	REQUIRE(1 == Invoke(NULL, "func1", true));
	REQUIRE(2 == Invoke(NULL, "func2", 3, 4));
	REQUIRE(3 == Invoke(NULL, "func3", 3, 4, "hello"));
}
