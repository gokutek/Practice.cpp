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


TEST_CASE("mutex", "[thread]")
{
	std::mutex mutex;
	REQUIRE(mutex.try_lock());
	mutex.unlock();
}


/*
===============================================================================
std::lock_guard与std::unique_lock的区别：
The difference is that you can lock and unlock a std::unique_lock. 
std::lock_guard will be locked only once on construction and unlocked on destruction.

std::unique_lock has other features that allow it to e.g.: be constructed 
without locking the mutex immediately but to build the RAII wrapper. However, 
std::unique_lock might have a tad more overhead.

std::lock_guard also provides a convenient RAII wrapper, but cannot lock
multiple mutexes safely. It can be used when you need a wrapper for a limited
scope, e.g.: a member function.

One of the differences between std::lock_guard and std::unique_lock is that
the programmer is able to unlock std::unique_lock, but she/he is not able to
unlock std::lock_guard.
===============================================================================
*/


TEST_CASE("lock_guard", "[thread]")
{
	std::mutex mutex;
	std::lock_guard<std::mutex> guard(mutex);
}
