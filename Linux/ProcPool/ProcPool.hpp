#pragma once
#include "Global.h"
#include "Pipe.hpp"

#include <random>
#include <optional>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// 仅简单实现了必须功能
class ProcPool
{
public:
	ProcPool() = default;
	ProcPool(const std::vector<std::function<void(void)>>& tasks)
	: _gen(std::random_device()()), _tasks(tasks)
	{}

	// 创建子进程, 同时子进程将进入读取任务的状态
	// 这里使用的规则是创建与任务数等量的进程, 实际可根据需求调整
	void CreateSubProcs()
	{
		size_t taskNum = _tasks.size();
		while(taskNum--)
		{
			// 创建管道, 每一个子进程对应一个管道
			int fd[2];
			int retPipe = pipe(fd);
			if(retPipe == -1) {perror("pipe failed");} // 如果创建管道失败

			/* 创建子进程
			 * 注意: 创建子进程时会拷贝父进程的文件描述符表
			 * ** 因此新的子进程的 文件描述符表 中将存在指向 前面的所有的子进程 对应的管道 的 写端 的 文件描述符
			 */
			pid_t retFork = fork();
			if(retFork == -1) {perror("fork failed");} // 如果创建子进程失败
			if(retFork == 0) // if 中的代码属于子进程
			{
				close(fd[1]); // 关闭当前子进程对应的管道文件的写端
				for(const auto& fdWR : _fdWRs) {close(fdWR);} // 关闭当前子进程从父进程"继承"的 指向其它子进程对应的管道文件的写端的文件描述符
				
				uint taskIndex;
				while(true) // 子进程进入读取任务的状态
				{
					size_t size = read(fd[0], &taskIndex, sizeof(taskIndex)); //从管道读取内容
					if(size == 4) // 其实读取的是一个 unsigned int 
					{
						std::cout << "SubProc: " << getpid() << " from ReadFd: " << fd[0] << ", Get Task[" << taskIndex << "]: ";
						_tasks[taskIndex]();// 调用任务函数
						std::cout << std::endl;
					}
					else if(size == 0)
					{
						std::cout << "None message to be read" << std::endl;
						break; // 跳出循环
					}
					else //(size == -1)
					{perror("failed to read");}
				}
				exit(0); // 子进程退出
			}

			// 以下属于父进程
			close(fd[0]);
			_pipes.emplace_back(fd[1], retFork);
			_fdWRs.emplace_back(fd[1]);
		}
	}
	
	// 这里简单采用随机的方式分配任务
	// 如果给taskCount 传递了非0, 则分配 taskCount 个任务后即停止分配任务, 否则表示执行无限个任务
	void RandomDistribution(uint taskCount)
	{
		size_t taskNum = _tasks.size();
		if(taskNum == 0)
		{
			std::cout << "None task to be sent!" << std::endl;
			return;
		}

		size_t subProcNum = _pipes.size();
		if(subProcNum == 0)
		{
			std::cout << "None proc!" << std::endl;
			return;
		}
		std::uniform_int_distribution<> distribTask(0, taskNum - 1); //随机数的范围 [0,  taskNum - 1]
		std::uniform_int_distribution<> distribPipe(0, subProcNum - 1); //随机数的范围 [0, subProcNum - 1]

		bool flag = taskCount == 0 ? false : true;
		while(true)
		{
			if(flag)
			{
				if(taskCount-- == 0) {break;} // 执行完成后跳出循环
			}

			uint taskIndex = distribTask(_gen); // 一个随机数, 用于表示要执行的任务的索引
			uint pipeIndex = distribPipe(_gen); //  一个随机数, 用于表示传递任务的管道索引,间接确定要执行任务的子进程
			//std::cout <<  taskIndex << pipeIndex << std::endl;

			std::cout << "Send Task[" << taskIndex << "] to pipe | " << _pipes[pipeIndex].GetName();
			fflush(stdout);
			int ret = write(_pipes[pipeIndex].GetfdWR(), &taskIndex, sizeof(taskIndex)); // 将任务索引, 写入管道
			if(ret == -1) {perror("failed to write");}
			sleep(1);
		}
		
		// 跳出循环后,依次关闭写端
		for(const auto& fdWR : _fdWRs) {close(fdWR);}
	}

	~ProcPool()
	{
		for(const auto& pipe : _pipes) 
		{
			std::cout << "Proc" << pipe.GetSubID() << " wait succeed..."<<std::endl;
			waitpid( pipe.GetSubID(), nullptr, 0);// 简单回收, 阻塞等待, 不获取退出信息
		}
	}
private:
	std::mt19937 _gen; //  Mersenne Twister 引擎, 用于生成随机数
	std::vector<std::function<void(void)>> _tasks;
	std::vector<Pipe> _pipes;
	std::vector<int> _fdWRs; // 存储的是主进程 指向管道文件的写端 的 文件描述符, 子进程将会继承, 在子进程中需要关闭(不关闭, 影响也不大)
};
