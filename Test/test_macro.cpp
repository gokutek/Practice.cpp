#include <iostream>


#define M1() std::cout << "M1" << std::endl;
#define M2() std::cout << "M2" << std::endl;
#define M3(a, b) a##b()
#define MM M


#define MACRO_EXPAND(m) m

#define M3_EX(a, b) MACRO_EXPAND(a)##MACRO_EXPAND(b)()

void test_macro()
{
	/*
	=================================================
	这个时候，MM不会先被替换为M，再展开为M1。也就是说，
	对于：#define M3(a, b) a##b()
	如果传递的宏参数也是宏，那么a##b时并不会进行宏替换。
	=================================================
	*/
	//M3(MM, 1)

	M3_EX(MM, 1)
}
