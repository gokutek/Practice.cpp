#include "unit_test.hpp"
#include "kapok/Kapok.hpp"

TEST_CASE(deserialize_with_wrong_key)
{
	using namespace kapok;
    int v = 0; 
    DeSerializer dr;
    dr.Parse(R"({"int":233})");

    bool flag = false;
    try
    {
        dr.Deserialize(v, "");
        TEST_REQUIRE(false && "should throw invalid_argument exception");
    }
    catch(std::invalid_argument&)
    {
        flag = true;
    }
    TEST_CHECK(flag && "should throw invalid_argument exception");
}

TEST_CASE(deserialize_with_invalid_string)
{
	using namespace kapok;
    DeSerializer dr;

    bool flag = false;
    try
    {
        dr.Parse(R"(this is a invalid string which can not be deserialized)");
        TEST_REQUIRE(false && "should throw invalid_argument exception");
    }
    catch(std::invalid_argument&)
    {
        flag = true;
    }
    TEST_CHECK(flag && "should throw invalid_argument exception");
    
}
