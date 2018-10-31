#include <time.h>
#include "catch.hpp"


/*
=========================
time的输入参数和返回值相同
=========================
*/
TEST_CASE("time", "time")
{
	time_t arg;
	time_t now = time(&arg);
	REQUIRE(arg == now);
}


/*
================================
可以使用strftime来格式化时间字符串
================================
*/
TEST_CASE("strftime", "time")
{
	time_t now = time(NULL);
	tm *ltm = localtime(&now);
	char strTime[64];
	strftime(strTime, sizeof(strTime), "%F %T", ltm);
	REQUIRE(strlen(strTime) == strlen("2018-10-31 13:08:42"));
}
