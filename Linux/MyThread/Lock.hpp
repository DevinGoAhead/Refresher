#ifndef __Lock
#define __Lock

#include "Global.h"

namespace wxy
{
	class Mutex
	{
	public:
		Mutex(const Mutex& x) = delete;
		Mutex& operator=(const Mutex& x) = delete;
	public:
		Mutex() : _pMutex(std::make_unique<pthread_mutex_t>())
		{
			int ret = pthread_mutex_init(_pMutex.get(), nullptr);
			if(ret != 0) {throw std::runtime_error("Failed to initialize mutex");}
		}
		void Lock()
		{
			int ret = pthread_mutex_lock(_pMutex.get());
			if(ret != 0) { throw std::runtime_error("Failed to lock");}
		}
		void Unlock()
		{
			int ret = pthread_mutex_unlock(_pMutex.get());
			if(ret != 0) {std::cerr << "Failed to Unlock: " << strerror(ret) << std::endl;}
		}
		~Mutex()
		{
			if(_pMutex != nullptr)
			{
				int ret = pthread_mutex_destroy(_pMutex.get());
				if (ret != 0) {std::cerr << "Failed to destroy mutex: " << strerror(ret) << std::endl;}
			}
		}
	private:
		std::unique_ptr<pthread_mutex_t> _pMutex;
	}; //end of class Lock

	class LockGuard
	{
	public:
		LockGuard(const LockGuard& x) = delete;
		LockGuard& operator=(const LockGuard& x) = delete;
	public:
		LockGuard(Mutex& mutex) : _mutex(mutex) 
		{_mutex.Lock();}

		~LockGuard(){_mutex.Unlock();}
	private:
		Mutex& _mutex; // 引用成员变量
	};//end of class LockGuard
}
#endif