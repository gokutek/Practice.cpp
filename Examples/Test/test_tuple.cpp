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
        
    int gpa1;
    char grade1;
    std::string name1;
    std::tie(gpa1, grade1, name1) = get_student(1);
    REQUIRE(gpa1 == 2);
    REQUIRE(grade1 == 'C');
    REQUIRE(name1 == "Milhouse Van Houten");
}
