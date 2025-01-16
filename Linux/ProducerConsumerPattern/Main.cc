#include "BlockQueue.hpp"
#include "Task.h"

#include <iostream>
//#include <ctime>
#include <random>

template<typename T>
T random(T first, T last)
{
	std::random_device rd; // 随机数生成器, 用于提供种子
	std::mt19937_64 gen(rd()); // 使用rd 生成的随机数初始化 Mersenne Twister 引擎对象
	std::uniform_int_distribution<T> distibute(first, last);
	return distibute(gen);
}

int add(int a, int b)
{
	return a + b;
}
int main()
{
	Calculator a(add, 4, 5);
	std::cout << a.Name() << std::endl;

}
	
