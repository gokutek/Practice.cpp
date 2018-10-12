#include <assert.h>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include "catch.hpp"


/*
======================
标准库中多线程相关工具
======================
*/


TEST_CASE("get_id", "[this_thread]")
{
    std::thread::id tid = std::this_thread::get_id();
    std::stringstream oss1;
    oss1 << tid;

    DWORD const dwThreadId = GetCurrentThreadId();
    std::stringstream oss2;
    oss2 << dwThreadId;

    REQUIRE(oss1.str() == oss2.str());
}


static void f2(int &n)
{
    for (int i = 0; i < 5; ++i) {
        ++n;
    }
}


TEST_CASE("thread", "[thread]")
{
    std::thread t1;
    REQUIRE(!t1.joinable());
    //t1.join(); // exception

    int N = 10;
    std::thread t2(f2, std::ref(N));
    t2.join();
    REQUIRE(N == 15);
}
