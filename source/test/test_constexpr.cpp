#include <array>
#include <vector>
#include <iostream>
#include "catch.hpp"

/*
===============================================================================
转自：
https://www.cnblogs.com/wangxiaobao/p/5966394.html
const：运行时常量
constexpr：编译时常量
===============================================================================
*/

constexpr int foo(int i)
{
    return i + 5;
}

class Point
{
public:
    constexpr Point(double xval = 0, double yval = 0) : x(xval), y(yval) {}
    constexpr double getX() const { return x; }
    constexpr double getY() const { return y; }

private:
    double x, y;
};

constexpr Point p1(9.4, 27);
constexpr Point p2(28.8, 5.3);

constexpr Point midpoint(const Point& p1, const Point& p2)
{
    return{ p1.getX() + p2.getX() / 2, p1.getY() + p2.getY() / 2 };
}

constexpr auto mid = midpoint(p1, p2);


constexpr int test_string()
{
    //std::string str; // 不能定义这个变量，error C3250: “str”: 不允许在“constexpr”函数体中进行声明
    return 10;
}


static constexpr int add(int x, int y)
{
    return x + y;
}


#if 0
// C++ 14
constexpr int factorial2(int n)
{
    int result = 1;
    for (int i = 1; i <= n; ++i)
        result *= i;
    return result;
}
#endif


TEST_CASE("constexpr", "[constexpr]")
{
    REQUIRE(3 == add(1, 2));

    int x;
    x = 10;
    REQUIRE(11 == add(1, x));

    std::array<int, foo(5)> arr; // OK，5是常量表达式，计算出foo(5)也是常量表达式
    arr;

    int i = 10;
    foo(i); // Call is Ok，i不是常量表达式，但仍然可以调用（constexpr 被忽略）
    //std::array<int, foo(i)> arr1; // Error，但是foo(i)的调用结果不是常量表达式了
}
