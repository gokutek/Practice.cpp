#include <sstream>
#include "catch.hpp"

/*
=====================
sstream中相关类的使用
=====================
*/

TEST_CASE("sstream clear", "sstream")
{
	std::ostringstream oss;
	oss << "hello";
	oss.clear(); // clear不是清理掉之前的输入
	oss << "world";

	REQUIRE(oss.str() == "helloworld");
}


TEST_CASE("sstream str", "sstream")
{
	std::ostringstream oss;
	oss << "hello";
	oss.str("");
	oss << "world";

	REQUIRE(oss.str() == "world");
}


/*
===============================================================================
1. 需使用空格或回车等分隔符隔开；
2. 字符串不能含有空格（可以对字符串进行base64编码后再插入）；
===============================================================================
*/
TEST_CASE("IO", "sstream")
{
	std::string const SEP = " ";

	bool bVal = true;
	int nVal = 123;
	int64_t i64Val = 456;
	float fVal = 12.3f;
	double dVal = 25.3;
	char const *str = "hello";

	std::ostringstream oss;
	oss << bVal << SEP << nVal << SEP << i64Val << SEP << fVal << SEP << dVal << SEP << str;
	std::string const buf = oss.str();

	std::istringstream iss;
	iss.str(buf);
	bool bVal1;
	int nVal1;
	int64_t i64Val1;
	float fVal1;
	double dVal1;
	std::string str1;
	iss >> bVal1 >> nVal1 >> i64Val1 >> fVal1 >> dVal1 >> str1;
	REQUIRE(bVal == bVal1);
	REQUIRE(nVal == nVal1);
	REQUIRE(i64Val == i64Val1);
	REQUIRE(fabs(fVal - fVal1) < FLT_EPSILON);
	REQUIRE(fabs(dVal - dVal1) < DBL_EPSILON);
	REQUIRE(str == str1);
}
