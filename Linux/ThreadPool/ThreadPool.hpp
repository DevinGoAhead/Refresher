#ifndef __ThreadPool
#define __ThreadPool

#include "Task.hpp"
#include "LockCond.hpp"
#include "Thread.hpp"

#include <vector>
#include <queue>
#include <tuple>

const uint ThreadCount = 3; // 线程池中线程的数量
// 本例只有一个生产者
// 本例不需要考虑任务队列满的情况
namespace wxy
{
	template<typename Task>
	class ThreadPool
	{
	public:
		ThreadPool() : _mutex(Mutex()), _cond(Cond()), _threadCount(ThreadCount)
		{
			for(uint i = 0; i <_threadCount; ++i)
			{
				// 相当于 new, 离开构造函数, 线程不会销毁
				// i 仅作为线程编号, 作为线程名称的组成部分
				auto pThread = std::make_shared<Thread>(i); 
				_pThreads.push_back(pThread);
			}
		}
		~ThreadPool(){}
		
		// 处理任务
		void PushTask(Task task)// 本例只有一个生产者, 因此这里不需要生产者同步, 只需要考虑互斥即可, 保证资源安全
		{
			{
				LockGuard lockGuard(_mutex);
				_tasks.push(task);
			}// 到这里LockGuard 释放锁
			_cond.Signal(); // 尝试唤醒一个消费者
		} 

		void Run()
		{
			for(auto& pThread : _pThreads)
			{
				// std::cout <<  std::string_view(typeid(decltype(this)).name()) << std::endl;
				// std::cout << std::string_view(typeid(decltype(pThread)).name()) << std::endl;

				auto pThreadCtx = std::make_shared<std::tuple<decltype(this), std::decay_t<decltype(pThread)>>>(this, pThread); // 这里会报错, 原因不得而知
				//auto pThreadCtx = std::make_shared<std::tuple<ThreadPool<Task>*, std::shared_ptr<Thread>>>(this, pThread);
				
				// 这里会先调用 Thread::Start, 通过 Thread::Start 调用 Thread::RoutineFunc, 通过 Thread::RoutineFunc 调用  ThreaPool::HandelTask
				pThread->Start(HandelTask, new auto(pThreadCtx)); 
				std::cout << pThread->Name() << " start ..." << std::endl;
			}
		}

	private:
		bool EmptyQueue()
		{return _tasks.empty();}
		
		// ThreadFunc
		static void* HandelTask(void* args)
		{
			auto ppThreadCtx = static_cast<std::shared_ptr<std::tuple<ThreadPool<Task>*, std::shared_ptr<Thread>>>*>(args);
			auto& ThreadCtx = *(*ppThreadCtx);
			auto& this_ = std::get<0>(ThreadCtx);
			auto& pThread = std::get<1>(ThreadCtx);
			delete ppThreadCtx;

			Task task;
			while(true)
			{
				{
					LockGuard lockGuard(this_->_mutex);
					while(this_->EmptyQueue()) // 如果空, 在这里阻塞, 唤醒后再次检查是否空, 避免伪唤醒
					{this_->_cond.Wait(this_->_mutex);}

					// 到这里一定是被唤醒了
					task =  this_->_tasks.front();
					this_->_tasks.pop();
				} // 到这里LockGuard 释放锁
				// 只有一个生产者, 不会在 cond 阻塞这里不需要唤醒生产者
				std::string result = task();
				std::cout << pThread->Name() << ": result after handling " << result << std::endl;
			}
			return nullptr;
		}
	private:
		std::vector<std::shared_ptr<Thread>> _pThreads;
		std::queue<Task> _tasks;
		Mutex _mutex;
		Cond _cond;
		
		uint _threadCount;
	};
}

#endif