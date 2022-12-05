/*
===========================================================
创建时间: 2022/12/5
功能说明: 测试第三方时间轮库ratas
===========================================================
*/

#include <assert.h>
#include <functional>
#include "timer-wheel.h"

int main()
{
	typedef std::function<void()> callback_t;
	int count = 0;
	TimerEvent<callback_t> callback([&count]() { ++count; });

	TimerWheel timers;
	timers.schedule(&callback, 5);
	timers.advance(4);
	assert(count == 0);
	timers.advance(1);
	assert(count == 1);

	//测试cancel
	timers.schedule(&callback, 5);
	callback.cancel();
	timers.advance(5);
	assert(count == 1);

	return 0;
}
