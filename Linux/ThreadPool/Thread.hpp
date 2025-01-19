#ifndef __Thread
#define __Thread

#include "Global.h"


#include <optional>
#include <string>
#include <sstream>
#include <utility>

namespace wxy
{
	class Thread
	{
	private:
		// 根据 一个可调用对象的 *类型*, 和参数类型, 推断返回值类型
		using Func_t = std::function<void*(void*)>;
	public:
		Thread(const Thread& x) = delete;
		Thread& operator=(const Thread& x) = delete;
	public:
		Thread(uint threadIndex)
		{
			//设置线程名称
			std::ostringstream oss;
			oss << "Thread#" << threadIndex;
			_name = oss.str();
		}

		void Start(Func_t func, void* args)
		{
			auto pFunc = std::make_shared<std::function<void*()>>
			(
				[ThreadFunc = func, argsCaptured = args]() -> void*
				{return ThreadFunc(argsCaptured);}
			);

			int ret = pthread_create(&_id, nullptr, RoutineFunc, new auto(pFunc));
			if(ret != 0) 
			{
				throw std::runtime_error("Failed to create thread");
			}
		}

		void Detach()
		{
			if(_isDetached == false)
			{
				int ret = pthread_detach(_id);
				if (ret != 0) {throw std::runtime_error("Failed to detach thread");}
			}
		}

		void* Join()
		{
			if(_isDetached) {throw std::logic_error("cannot join a detached thread");}
			if(_isJoined) {throw std::logic_error("Thread already joined");}
			
			void* retval = nullptr;
			int ret = pthread_join(_id, &retval);
			if (ret != 0) {throw std::runtime_error("Failed to join thread");}

			_isJoined = true;
			return  retval;
		}

		std::string& Name() {return _name;}

		~Thread()
		{
			if(_isDetached == false && _isJoined == false) // 说明线程资源还没有回收
			{

				try{Join();}
				catch(...){std::cerr << "Failed to join in destructor"  << std::endl;}
			} 
		}

	private:
		// 参数只能有void*, 因此 static, 屏蔽 this
		static void* RoutineFunc(void* arg)
		{
			auto ppFunc = static_cast<std::shared_ptr<std::function<void*()>>*>(arg); //由 void* 转换为 shared_ptr*
			auto& Func = *(*ppFunc);
			Func(); // 直接调用 function 对象
			//delete ppFunc;
			return nullptr;
		}
	private:
		pthread_t _id; // Thread id
		bool _isDetached;
		bool _isJoined;
		std::string _name; // Thread name
	}; // end of class Thread 
}
#endif
