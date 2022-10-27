#include "test_var_list.h"
#include "catch.hpp"

TEST_CASE("VarList", "VarList")
{
	VarList varlist;
	varlist.AddBool(true);
	varlist.AddInt(666);
	varlist.AddInt64(888);
	varlist.AddFloat(1.23f);
	varlist.AddDouble(3.1415926);
	varlist.AddString("hello");

	REQUIRE(varlist.GetCount() == 6);
	REQUIRE(varlist.GetType(0) == VT::VT_BOOL);
	REQUIRE(varlist.GetType(1) == VT::VT_INT);
	REQUIRE(varlist.GetType(2) == VT::VT_INT64);
	REQUIRE(varlist.GetType(3) == VT::VT_FLOAT);
	REQUIRE(varlist.GetType(4) == VT::VT_DOUBLE);
	REQUIRE(varlist.GetType(5) == VT::VT_STRING);

	REQUIRE(varlist.BoolVar(0) == true);
	REQUIRE(varlist.IntVar(1) == 666);
	REQUIRE(varlist.Int64Var(2) == 888);
	REQUIRE(fabs(varlist.FloatVar(3) - 1.23f) < FLT_EPSILON);
	REQUIRE(fabs(varlist.DoubleVar(4) - 3.1415926) < DBL_EPSILON);
	REQUIRE(varlist.StringVar(5) == std::string("hello"));

	char const *p1 = varlist.StringVar(5);
	char const *p2 = varlist.StringVar(5);
	REQUIRE(p1 == p2);
}
