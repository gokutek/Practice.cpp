#ifndef test_constexpr_h
#define test_constexpr_h

#include <array>

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

constexpr
Point midpoint(const Point& p1, const Point& p2)
{
    return{ p1.getX() + p2.getX() / 2, p1.getY() + p2.getY() / 2 };
}

constexpr auto mid = midpoint(p1, p2);

inline void test_constexpr()
{
#if 0
    int i;
    const int size = i;
    int arr[size];         // 编译错误，size不是常量表达式，不能在编译期确定
#endif

    int i = 10;
    std::array<int, foo(5)> arr; // OK，5是常量表达式，计算出foo(5)也是常量表达式
    arr;
    foo(i); // Call is Ok，i不是常量表达式，但仍然可以调用（constexpr 被忽略）
    //std::array<int, foo(i)> arr1; // Error，但是foo(i)的调用结果不是常量表达式了
}

#endif // test_constexpr_h
