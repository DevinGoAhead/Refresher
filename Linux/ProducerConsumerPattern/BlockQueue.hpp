#include <queue>

const int maxNum = 5;

template<typename T>
class BufferQueue // 任务缓冲队列
{
public:
	BufferQueue() : _que(std::queue<T>())
	{}
	const T& Front() {return _que.front();}
	void Push(const T& task) {_que.push(task);}
	void Pop(){_que.pop();}
private:
	std::queue<T> _que;
};