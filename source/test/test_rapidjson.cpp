#include <iostream>
#include <float.h>
#include <string>
#include "catch.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


TEST_CASE("read json", "rapidjson")
{
    std::string const strJSON = 
    "{\
        \"hello\": \"world\",\
        \"t\" : true,\
        \"f\" : false,\
        \"n\" : null,\
        \"i\" : 123,\
        \"pi\" : 3.1416,\
        \"a\" : [1, 2, 3, 4]\
    }";

    rapidjson::Document document;
    document.Parse(strJSON.c_str());

    REQUIRE(document.IsObject());

    REQUIRE(document["hello"].IsString());
    REQUIRE(!strcmp(document["hello"].GetString(), "world"));

    REQUIRE(document["t"].GetBool() == true);

    REQUIRE(document["f"].GetBool() == false);

    REQUIRE(document["n"].IsNull());

    REQUIRE(document["i"].GetInt() == 123);

    REQUIRE(fabs(document["pi"].GetFloat() - 3.1416f) < FLT_EPSILON);

    REQUIRE(document["a"].IsArray());
    rapidjson::Value const &a = document["a"];
    REQUIRE(a.Size() == 4);
    for (rapidjson::SizeType i = 0; i < a.Size(); ++i) {
        REQUIRE(i + 1 == a[i].GetInt());
    }

    REQUIRE(document.HasMember("pi"));
    REQUIRE(!document.HasMember("shit"));
}


TEST_CASE("write json", "rapidjson")
{
    rapidjson::Document document;
    document.SetObject();
    
    rapidjson::Value key("key", document.GetAllocator());
    rapidjson::Value val(42);
    document.AddMember(key, val, document.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string const strJSON = buffer.GetString();
    REQUIRE(strJSON == "{\"key\":42}");
}
