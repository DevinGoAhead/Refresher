#ifndef __Thread
#define __Thread

#include "Global.h"

#include <functional>
#include <optional>
#include <string>
#include <sstream>
#include <utility>

namespace wxy
{
	class Thread;
	struct ThreadContext
	{
		Thread* _cThis;
		void* _cArg;
	};
	class Thread
	{
	private:
		using RoutineFunc = std::function<void*(void*)>;
	public:
		Thread(const Thread& x) = delete;
		Thread& operator=(const Thread& x) = delete;
	public:
		Thread(RoutineFunc routineFunc, void* arg, unsigned int threadIndex) 
		: _routineFunc(routineFunc), _context(std::make_unique<ThreadContext>()), _isDetached(false), _isJoined(false)
		{
			
			// 将 this 和 arg 放到 ThreadContext 对象中
			// unique_ptr 可以使用 -> 直接访问成员, 不需要 get() 函数
			_context->_cArg = arg;
			_context->_cThis = this;

			// 这里会调用 StartRoutine,并以  _context.get() 作为参数
			// c++ 中任何指针都可以隐式转换为 void*, 足够安全, 不需要 显示转换
			int ret = pthread_create(&_id, nullptr, StartRoutine, _context.get());
			if(ret != 0) {throw std::runtime_error("Failed to create thread");}
			
			//设置线程名称
			std::ostringstream oss;
			oss << "Thread#" << threadIndex;
			_name = oss.str();
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
		std::string& GetName() {return _name;}
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
		// 但是, 这样又无法访问 _arg, 故引入类型 ThreadContext
		static void* StartRoutine(void* arg)
		{
			ThreadContext* pThreadCtx = static_cast<ThreadContext*>(arg); // arg 是 void* 类型
			return pThreadCtx->_cThis->Run(pThreadCtx->_cArg); // 不使用Run, 直接调用 _routineFunc(pThreadCtx->c_arg) 也可以
		}

		void* Run(void* arg) {return _routineFunc(arg);}
	private:
		pthread_t _id; // Thread id
		RoutineFunc _routineFunc;
		std::unique_ptr<ThreadContext> _context;
		bool _isDetached;
		bool _isJoined;
		std::string _name; // Thread name
	}; // end of class Thread 
}
#endif
