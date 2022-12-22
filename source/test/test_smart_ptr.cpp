/*
 * 2018/10/12: 文件创建
 */
 
#include <memory>
#include "catch.hpp"


/*
========================
标准库中智能指针相关工具
========================
*/


namespace
{
    struct Foo
    {
        explicit Foo(int _val) : val(_val) {}
        int val;
    };
}


TEST_CASE("test for non-null pointer", "[unique_ptr]")
{
    std::unique_ptr<Foo> p(new Foo(10));
    REQUIRE(p->val == 10);

    REQUIRE(p);

    p.reset();
    REQUIRE(!p);
}


TEST_CASE("release", "[unique_ptr]")
{
    std::unique_ptr<Foo> p(new Foo(10));
    REQUIRE(p->val == 10);

    Foo *p1 = p.release();
    REQUIRE(!p);

    REQUIRE(p1->val == 10);
    delete p1;
}


TEST_CASE("swap", "[unique_ptr]")
{
    std::unique_ptr<Foo> up1(new Foo(1));
    std::unique_ptr<Foo> up2(new Foo(2));

    up1.swap(up2);

    REQUIRE(up1->val == 2);
    REQUIRE(up2->val == 1);
}
