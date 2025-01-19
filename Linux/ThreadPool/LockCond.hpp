#ifndef __Lock
#define __Lock

#include "Global.h"

namespace wxy
{
	class Cond;
	class Mutex
	{
	public:
		friend Cond;
	public:
		Mutex(const Mutex& x) = delete;
		Mutex& operator=(const Mutex& x) = delete;
	public:
		Mutex() : _pMutex(std::make_unique<pthread_mutex_t>())
		{
			int ret = pthread_mutex_init(_pMutex.get(), nullptr);
			if(ret != 0) {throw std::runtime_error("Failed to initialize mutex");}
		}

		Mutex(Mutex&& x) : _pMutex(std::move(x._pMutex))
		{x._pMutex = nullptr;}

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

	// class Condition Variable
	class Cond
	{
	public:
		Cond() : _pCond(std::make_unique<pthread_cond_t>())
		{
			int ret = pthread_cond_init(_pCond.get(), nullptr);
			if(ret != 0) {throw std::runtime_error("Failed to initialize cond");}
		}

		Cond(Cond&& x) : _pCond(std::move(x._pCond))
		{x._pCond = nullptr;}

		void Wait(Mutex& mutex)
		{
			int ret = pthread_cond_wait(_pCond.get(), mutex._pMutex.get());
			if(ret != 0) {throw std::runtime_error("Error occurred when waitting on cond");}
		}

		void Signal()
		{
			int ret = pthread_cond_signal(_pCond.get());
			if(ret != 0) {throw std::runtime_error("Error occurred when cancling blocked on cond");}
		} 
		
		~Cond()
		{
			int ret = pthread_cond_destroy(_pCond.get());
			if(ret != 0) {std::cerr << "Failed to destroy cond" << std::endl;}
		} 
	private:
		std::unique_ptr<pthread_cond_t> _pCond;
	}; // end of class Cond
}
#endif