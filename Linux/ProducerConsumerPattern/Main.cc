#include "BufferQueue.hpp"
#include "Task.hpp"

#include <array>
#include <random>
#include <vector>
#include <memory>
#include <type_traits>



// 生成一个范围内的随机数
template<typename T>
T RandomInteger(T first, T last)
{
	std::random_device rd; // 随机数生成器, 用于提供种子
	std::mt19937_64 gen(rd()); // 使用rd 生成的随机数初始化 Mersenne Twister 引擎对象
	std::uniform_int_distribution<T> distribute(first, last);
	return distribute(gen);
}

// 计算 + - * /
template<typename T>
std::optional<T> CalFunc(T x, T y, char op)
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

const std::array<char, 4ul> ops{'+', '-', '*', '/'}; // 随机从这里取运算符
using BufferQueue_t = BufferQueue<CalTask<int>>;
//using ThreadCtx_t = std::tuple<BufferQueue_t, pthread_mutex_t, pthread_cond_t>;

void* ProduceCalTask(void* args)
{
	auto ppBufQue = static_cast<std::shared_ptr<BufferQueue_t>*>(args);
	auto& bufQue = *(*ppBufQue);
	delete ppBufQue; // 清理指向 shared_ptr 的指针
	while(true)
	{
		char opIndex = RandomInteger(0, 3);
		int x = RandomInteger(0, 30);
		int y = RandomInteger(0, 10);
		std::function<std::optional<int>(int, int, char)> calFunc = CalFunc<int>;
		CalTask<int> calTask(x, y, ops[opIndex], calFunc);
		std::string taskName(calTask.Name());
		std::cout <<" ProduceTask: " << taskName << std::endl;
		
		bufQue.Push(calTask);
		sleep(1);
	}
	return nullptr;
}

void* ConsumerCalTask(void* args)
{
	auto ppBufQue = static_cast<std::shared_ptr<BufferQueue_t>*>(args);
	auto& bufQue = *(*ppBufQue);
	delete ppBufQue; // 清理指向 shared_ptr 的指针
	while(true)
	{
		CalTask<int> calTask;
		//std::cout << "ConsumerCalTask1" << std::endl;
		bufQue.Pop(&calTask); 
		//std::cout << "ConsumerCalTask2" << std::endl;
		std::string resultName(calTask());
		std::cout <<" ConsumerTask: " << resultName << std::endl;
		
		sleep(1);
	}
	return nullptr;
}

int main()
{
	BufferQueue_t bufQue; // 一个公共的缓冲队列, 用于初始任务存取
	
	auto pBufQue = std::make_shared<BufferQueue_t>(bufQue); // 智能指针管理
	std::vector<pthread_t> tidsPd(5); // 存放生产计算任务的生产者线程id
	std::vector<pthread_t> tidsCs(4); // 存放处理计算任务的消费者线程id
	
	for(auto& tid : tidsPd) {pthread_create(&tid, nullptr, ProduceCalTask, static_cast<void*>(new auto(pBufQue)));} // 创建生产者线程
	for(auto& tid : tidsCs) {pthread_create(&tid, nullptr, ConsumerCalTask, static_cast<void*>(new auto(pBufQue)));} // 创建消费者线程

	while(true)
	{
		// 先不退出
	}
}
