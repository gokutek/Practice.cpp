#include <tuple>
#include "catch.hpp"


/*
======================
std::tuple
======================
*/


static std::tuple<int, char, std::string> get_student(int id)
{
    if (id == 0) return std::make_tuple(3, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2, 'C', "Milhouse Van Houten");
    if (id == 2) return std::make_tuple(1, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}


TEST_CASE("tuple", "[tuple]")
{
    auto student0 = get_student(0);
    REQUIRE(std::get<0>(student0) == 3);
    REQUIRE(std::get<1>(student0) == 'A');
    REQUIRE(std::get<2>(student0) == "Lisa Simpson");
        
    int gpa;
    char grade;
    std::string name;
    std::tie(gpa, grade, name) = get_student(1);
    REQUIRE(gpa == 2);
    REQUIRE(grade == 'C');
    REQUIRE(name == "Milhouse Van Houten");
}


TEST_CASE("C++17 structured binding", "[tuple]")
{
	auto[gpa, grade, name] = get_student(2);
	REQUIRE(gpa == 1);
	REQUIRE(grade == 'D');
	REQUIRE(name == "Ralph Wiggum");
}
