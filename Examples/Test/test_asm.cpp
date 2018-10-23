﻿#ifdef _MSC_VER

#include "catch.hpp"

/*
========
内联汇编
========
*/

TEST_CASE("assembly.1", "[ASM]")
{
	int count = 12;

	__asm
	{
		mov eax, count
		mov ebx, 85
		add eax, ebx
		mov count, eax
	}

	REQUIRE(count == 97);
}


static int func1(int a, int b)
{
	return a + b;
}


TEST_CASE("assembly.2", "[ASM]")
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

	REQUIRE(res == 25);
}


static int __stdcall func2(int a, int b)
{
	return a + b;
}


TEST_CASE("assembly.3", "[ASM]")
{
	int res;

	__asm
	{
		push 12
		push 13
		call func2
		mov res, eax
	}

	REQUIRE(res == 25);
}

#endif // _MSC_VER
