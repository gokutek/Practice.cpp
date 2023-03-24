/*
 * 2023/03/24: 整理编译
 */

#include <Windows.h>
#include <assert.h>

#ifndef _AMD64_
/*
========
内联汇编
========
*/

static void test_asm1()
{
	int count = 12;

	__asm
	{
		mov eax, count
		mov ebx, 85
		add eax, ebx
		mov count, eax
	}

	assert(count == 97);
}


/*
===============================================================================
__cdecl，需要调用者来恢复ESP
===============================================================================
*/
static int func1(int a, int b)
{
	return a + b;
}

static void test_asm2()
{
	int res;

	__asm
	{
		push 12
		push 13
		call func1
		mov res, eax
		add esp, 8
	}

	assert(res == 25);
}


/*
===============================================================================
__stdcall，不需要调用者来恢复ESP，函数ret时会自己恢复ESP
===============================================================================
*/
static int __stdcall func2(int a, int b)
{
	return a + b;
}

static void test_asm3()
{
	int res;

	__asm
	{
		push 12
		push 13
		call func2
		mov res, eax
	}

	assert(res == 25);
}

#endif // _AMD64_

int main()
{
#ifndef _AMD64_
	test_asm1();
	test_asm2();
	test_asm3();
#endif //_AMD64_
	return 0;
}
