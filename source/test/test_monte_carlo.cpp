#include <random>
#include <iostream>
#include <time.h>

/*
===============================================================================
假设被积函数是x^2，积分区间是[3,6]，解析方式求解结果是63.

用蒙特卡罗方法求算$\int_{a}^{b}f(x)dx$的步骤：
1，在积分区间$[a,b]$内使用均匀分布随机生成$n$个采样点；
2，计算$F_n=\frac{b-a}{n}\sum_{i=1}^{n}f(X_i)$，作为积分的近似值。
===============================================================================
*/

double INT_LOWER_BOUND = 3;
double INT_UPPER_BOUND = 6;

static double fx(double x)
{
	return x * x;
}

static double calc(std::default_random_engine& engine, std::uniform_real_distribution<double>& uniform, int samples)
{
	double result = 0;
	double factor = 1.0f * (INT_UPPER_BOUND - INT_LOWER_BOUND) / samples;
	for (int i = 0; i < samples; ++i)
	{
		double x = uniform(engine);
		result += factor * fx(x);
	}
	return result;
}

int main()
{
	std::default_random_engine engine((unsigned int)time(nullptr));
	std::uniform_real_distribution<double> uniform(INT_LOWER_BOUND, INT_UPPER_BOUND);
	
	std::cout << calc(engine, uniform, 1000) << std::endl;
	std::cout << calc(engine, uniform, 10000) << std::endl;
	std::cout << calc(engine, uniform, 100000) << std::endl;
	std::cout << calc(engine, uniform, 1000000) << std::endl;
	std::cout << calc(engine, uniform, 10000000) << std::endl;
	std::cout << calc(engine, uniform, 100000000) << std::endl;

	return 0;
}
