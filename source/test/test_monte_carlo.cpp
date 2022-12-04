#include <random>
#include <iostream>
#include <time.h>

/*
===============================================================================
假设被积函数是x^2，积分区间是[1,10]，解析方式求解结果是333
===============================================================================
*/

int main()
{
	srand((unsigned int)time(nullptr));
	
	int const SAMPLER_COUNT = 1000000;
	float result = 0;
	for (int i = 0; i < SAMPLER_COUNT; ++i)
	{
		int x = (rand()%10+1);
		result += x*x;
	}
	std::cout << 1.0f*result/SAMPLER_COUNT*10 << std::endl;

	return 0;
}
