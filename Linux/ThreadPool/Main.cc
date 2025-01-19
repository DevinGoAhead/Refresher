#include "ThreadPool.hpp"

#include <array>
#include <random>
#include <unistd.h>

using namespace wxy;

// 计算 + - * /
std::optional<int> CalFunc(int x, int y, char op)
{
	switch (op)
	{
		case '+':
			return x + y;
		case '-':
			return x - y;
		case '*':
			return x * y;
		case '/':
			{
				if(y != 0) {return x / y;}
				else return std::nullopt;
			}
		default:
			return std::nullopt;
	}
}

int main()
{
	using CalFunc_t = std::function<std::optional<int>(int, int, char)>;
	CalFunc_t calFunc = CalFunc;
	
	int x, y;
	char op;
	
	ThreadPool<CalTask<int>> threadPool;
	threadPool.Run();

	while(true)
	{
		std::cout << "Please input x: ";
		std::cin >> x;

		std::cout << "Please input operator: ";
		std::cin >> op;

		std::cout << "Please input y: ";
		std::cin >> y;

		CalTask<int> calTask(x, y, op, calFunc);
		threadPool.PushTask(calTask);
		usleep(500);
	}
}