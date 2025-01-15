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
	template<typename F, typename... Args>
	class Thread
	{
	private:
		// 根据 一个可调用对象的 *类型*, 和参数类型, 推断返回值类型
		using ReturnType = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;
	public:
		Thread(const Thread& x) = delete;
		Thread& operator=(const Thread& x) = delete;
	public:
		Thread(/*uint threadIndex,*/ F&& func, Args&&... args)
		{
			// [ThreadFunc = std::forward<F>(func), ... argsCaptured = std::forward<Args>(args)] #捕获对象列表
			// ()->ReturnType #lambda 表达式无参, 返回值类型为 ->ReturnType
			// {func(argsCaptured)} # lambda 表达式的函数体,使用捕获的对象, 将args 传递给可调用对象 ThreadFunc
			// auto shrPtrTaskFunc = std::make_shared<std::function<ReturnType()>>/*创建一个shared ptr, 指向一个 无参,  返回值类型为 ->ReturnType 的 function 类型的对象 */
			// (
			// 	[ThreadFunc = std::forward<F>(func), ... argsCaptured = std::forward<Args>(args)]()->ReturnType
			// 	{return ThreadFunc(argsCaptured...);}
			// );
			 auto shrPtrTaskFunc = std::make_shared<std::function<ReturnType()>>(
            [func = std::forward<FF>(func), captured_args = std::make_tuple(std::forward<AA>(args)...)]() -> ReturnType {
                return std::apply(func, captured_args); // 在调用时使用 std::apply 和 std::forward
            });

			// 将 shared_ptr 转换为 void* 并传递给 pthread_create
			int ret = pthread_create(&_id, nullptr, StartRoutine, static_cast<void*>(shrPtrTaskFunc));
			if(ret != 0) 
			{
				//delete shrPtrTaskFunc;
				throw std::runtime_error("Failed to create thread");
			}
			
			//设置线程名称
			std::ostringstream oss;
			//oss << "Thread#" << threadIndex;
			oss << "Thread#" << 1;
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

			_isJoined == true;
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
		static void* StartRoutine(void* arg)
		{
			auto shrPtrTaskFunc = static_cast<std::shared_ptr<std::function<ReturnType()>>>(arg); //由 void* 转换为 shared_ptr
			(*shrPtrTaskFunc)(); // 直接调用 function 对象
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
