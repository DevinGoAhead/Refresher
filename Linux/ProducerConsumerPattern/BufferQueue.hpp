#ifndef __BufferQueue
#define __BufferQueue

#include "Global.h"

#include <queue>

const int maxNum = 5;

template<typename T>
class BufferQueue // 任务缓冲队列
{
public:
	BufferQueue() : _que(std::queue<T>()){
		// 锁和条件变量初始化
		pthread_mutex_init(&_mutex, nullptr);
		pthread_cond_init(&_condPd, nullptr);
		pthread_cond_init(&_condCs, nullptr);
	}
	void Push(const T& task) {
		//访问临界资源
		pthread_mutex_lock(&_mutex);
		while(Full()) {//while 避免伪唤醒
			pthread_cond_wait(&_condPd, &_mutex); // 在这里阻塞, 等待唤醒
		}
		
		_que.push(task);// 到这里一定是非 full 的, 且一定有一个生产者被唤醒了

		pthread_mutex_unlock(&_mutex); // 释放锁
		pthread_cond_signal(&_condCs); // 在条件变量阻塞位置唤醒一个消费者, 因为至少有一个任务可以处理了
	}
	
	template<typename... Args>
	void Emplace(Args... args) {_que.emplace(std::forward<Args>(args)...);}
	
	void Pop(T* x) {
		//访问临界资源
		pthread_mutex_lock(&_mutex);
		while(Empty()) {//while 避免伪唤醒
			pthread_cond_wait(&_condCs, &_mutex); // 在这里阻塞, 等待唤醒
		}
		*x = _que.front();
		_que.pop();// 到这里一定是非 empty 的, 且一定有一个消费者被唤醒了
		pthread_mutex_unlock(&_mutex); // 释放锁
		pthread_cond_signal(&_condPd); // 在条件变量阻塞位置唤醒一个生产者, 因为缓冲队列中至少有一个空间可以存放任务了
	}

	bool Empty() {return _que.empty();}
	bool Full() {return _que.size() > maxNum;}
private:
	std::queue<T> _que; // 一个公共的缓冲队列, 用于初始任务存取
	pthread_mutex_t _mutex; // 一把公共锁;
	pthread_cond_t _condPd; // 消费者条件变量
	pthread_cond_t _condCs; // 生产者件变量
};

#endif