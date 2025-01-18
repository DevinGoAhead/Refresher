#include "BufferQueue.hpp"
#include "Task.hpp"

#include <array>
#include <random>
#include <vector>
#include <memory>
#include <type_traits>
#include <fstream>

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

// 将计算结果存入文件
void SaveFunc(const std::string& pathName, const std::string& result)
//void SaveFunc(std::string pathName, std::string result)
{
	// std::ofstream ofs(pathName, std::ios::app);
	// if(!ofs) // 重载了 !, 检测是否设置了 badbit failbit, 这里主要检测是否打开成功
	// {
	// 	std::cerr << "Failed to open file" << std::endl;
	// 	return;
	// }
	// ofs << result << std::endl;

	// if(!ofs)// 这里主要检测是否写入成功
	// {std::cerr << "Failed to write to file" << std::endl;}
	FILE* fp = fopen(pathName.c_str(), "a+");
    if(fp == nullptr)
    {
        perror("fopen failed");
        exit(1);
    }
    fputs(result.c_str(), fp);
    fputs("\n", fp);
    //std::cout<<"succeed"<<std::endl;
    fclose(fp);
}

using CalFunc_t = std::function<std::optional<int>(int, int, char)>;
using SaveFunc_t = std::function<void(std::string, std::string)>;

using CalTask_t = CalTask<std::optional<int>, CalFunc_t, int, int, char>;
using SaveTask_t = SaveTask<void, SaveFunc_t, std::string, std::string>;

using BufferQueueCal_t = BufferQueue<CalTask_t>;
using BufferQueueSv_t = BufferQueue<SaveTask_t>;

const std::array<char, 4ul> ops{'+', '-', '*', '/'}; // 随机从这里取运算符
std::string PathName = "./Results.txt";

void* ProduceCalTask(void* args)
{
	auto ppBufQues = static_cast<std::shared_ptr<std::tuple<BufferQueueCal_t, BufferQueueSv_t>>*>(args);
	//auto bufQue = *(*ppBufQue);// 这样会产生缓冲队列的拷贝, 导致有多个缓冲队列
	auto& taskBufQue = std::get<0>(*(*ppBufQues));
	delete ppBufQues; // 清理指向 shared_ptr 的指针

	while(true)
	{
		char opIndex = RandomInteger(0, 3);
		int x = RandomInteger(0, 30);
		int y = RandomInteger(0, 10);
		//std::function<std::optional<int>(int, int, char)> calFunc = CalFunc<int>;
		char op = ops[opIndex];
		CalTask_t calTask(CalFunc, std::move(x), std::move(y), std::move(op));
		//CalTask_t calTask(CalFunc, x, y,op);
		std::string taskName(calTask.Name());
		std::cout <<" ProduceTask: " << taskName << std::endl;
		
		taskBufQue.Push(calTask);
		sleep(1);
	}
	return nullptr;
}

void* HandleCalTask(void* args)
{
	auto ppBufQues = static_cast<std::shared_ptr<std::tuple<BufferQueueCal_t, BufferQueueSv_t>>*>(args);
	//auto bufQue = *(*ppBufQue);// 这样会产生缓冲队列的拷贝, 导致有多个缓冲队列
	auto& bufQues = *(*ppBufQues);
	auto& taskBufQue = std::get<0>(bufQues);
	auto& retBufQue = std::get<1>(bufQues);
	delete ppBufQues; // 清理指向 shared_ptr 的指针

	while(true)
	{
		// 处理计算任务
		CalTask_t calTask;
		taskBufQue.Pop(&calTask);

		std::string result(calTask());
		std::cout <<" HandleCalTask: " << result << std::endl;
		
		// 将计算结果 push 到缓冲队列
		SaveTask_t saveTask(SaveFunc, std::move(PathName), std::move(result));
		//SaveTask_t saveTask(SaveFunc, PathName, result);
		retBufQue.Push(saveTask);
		
		sleep(1);
	}
	return nullptr;
}

void* SaveRetTask(void* args)
{
	auto ppBufQues = static_cast<std::shared_ptr<std::tuple<BufferQueueCal_t, BufferQueueSv_t>>*>(args);
	//auto bufQue = *(*ppBufQue);// 这样会产生缓冲队列的拷贝, 导致有多个缓冲队列
	auto& bufQue = *(*ppBufQues);
	auto& retBufQue = std::get<1>(bufQue);
	delete ppBufQues; // 清理指向 shared_ptr 的指针
	while(true)
	{
		SaveTask_t saveTask;

		retBufQue.Pop(&saveTask); 

		std::string writeMsg(saveTask());
		std::cout <<" SaveRetTask: " << writeMsg << std::endl;
		
		sleep(1);
	}
	return nullptr;
}

int main()
{
	BufferQueueCal_t taskBufQue; // 一个公共的缓冲队列, 用于初始任务存取
	BufferQueueSv_t RetBufQue; // 一个公共的缓冲队列, 用于计算结果存取

	auto pBufQues = std::make_shared<std::tuple<BufferQueueCal_t, BufferQueueSv_t>>(taskBufQue, RetBufQue); // 智能指针管理

	std::vector<pthread_t> tidsPrd(1); // 存放生产计算任务并放入原始任务缓冲队列的线程的id, 仅是生产者
	std::vector<pthread_t> tidsCalSav(1); // 存放处理计算任务并放入计算结果缓冲队列的线程的id, 既是消费者, 又是生产者
	std::vector<pthread_t> tidsSav(1); // 存放从计算结果缓冲队列取出结果并写入文件的线程的id, 仅是消费者c
	
	for(auto& tid : tidsPrd) {pthread_create(&tid, nullptr, ProduceCalTask, static_cast<void*>(new auto(pBufQues)));} // 创建任务生产线程
	for(auto& tid : tidsCalSav) {pthread_create(&tid, nullptr, HandleCalTask, static_cast<void*>(new auto(pBufQues)));} // 创建任务计算线程
	for(auto& tid : tidsSav) {pthread_create(&tid, nullptr, SaveRetTask, static_cast<void*>(new auto(pBufQues)));} // 创建结果保存线程

	while(true)
	{
		// 先不退出
	}
}
