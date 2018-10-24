/*
===============================================================================
在每个编译单元（cpp），__COUNTER__都是从0开始计数，也就是说不同的cpp中，
__COUNTER__会出现相同的值
===============================================================================
*/

static int i = __COUNTER__;
static int j = __COUNTER__;
static int k = __COUNTER__;


#include "catch.hpp"


#define M1() "M1"
#define M2() "M2"
#define M3(a, b) a##b()
#define MM M


#define MACRO_EXPAND(...) __VA_ARGS__

#define M3_EX(a, b) MACRO_EXPAND(a)##MACRO_EXPAND(b)()


TEST_CASE("macro expand", "[macro]")
{
	/*
	==============================================================================
	这个时候，MM不会先被替换为M，再展开为M1。也就是说，
	对于：#define M3(a, b) a##b()
	如果传递的宏参数也是宏，那么a##b时并不会进行宏替换。
	推广开来，如果在定义宏的时候，期望宏的参数也是宏，那么就需要使用上面MACRO_EXPAND这
	样的手段。
	==============================================================================
	*/
	//M3(MM, 1)

    std::string const str = M3_EX(MM, 1);
    REQUIRE(str == "M1");
}


TEST_CASE("COUNTER", "[COUNTER]")
{
	REQUIRE(i == 0);
	REQUIRE(j == 1);
	REQUIRE(k == 2);
}
