/*
 * 2018/10/12: 文件创建
 */
 
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


/*
===============================================================================
The effects of notify_one()/notify_all() and each of the three atomic parts of 
wait()/wait_for()/wait_until() (unlock+wait, wakeup, and lock) take place in a 
single total order that can be viewed as modification order of an atomic 
variable: the order is specific to this individual condition_variable. This 
makes it impossible for notify_one() to, for example, be delayed and unblock a
thread that started waiting just after the call to notify_one() was made.

The notifying thread does not need to hold the lock on the same mutex as the 
one held by the waiting thread(s); in fact doing so is a pessimization, since
the notified thread would immediately block again, waiting for the notifying 
thread to release the lock. However, some implementations (in particular many 
implementations of pthreads) recognize this situation and avoid this "hurry up 
and wait" scenario by transferring the waiting thread from the condition 
variable's queue directly to the queue of the mutex within the notify call, 
without waking it up.

Notifying while under the lock may nevertheless be necessary when precise 
scheduling of events is required, e.g. if the waiting thread would exit the 
program if the condition is satisfied, causing destruction of the notifying 
thread's condition_variable. A spurious wakeup after mutex unlock but before 
notify would result in notify called on a destroyed object.
===============================================================================
*/
TEST_CASE("notify_one", "[condition_variable]")
{
}
