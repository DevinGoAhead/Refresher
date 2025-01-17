#ifndef __Task
#define __Task

#include "Global.h"

#include <sstream>

// 计算: 从原始任务缓冲队列中获取任务并进行计算
// 存储: 将计算完成的任务放到存储计算结果的缓冲队列
template<typename T>
class CalTask
{
public:
	using TaskFunc_t = std::function<std::optional<T>(T, T, char)>;
public:
	CalTask() = default;
	CalTask(T x, T y, char op, TaskFunc_t TaskFunc) : _x(x), _y(y), _op(op), _TaskFunc(TaskFunc)
	{
		if (!TaskFunc) {
            std::cerr << "Error: calFunc is not initialized properly!" << std::endl;
        }
		if (!_TaskFunc) {
            std::cerr << "Error: calFunc is not initialized properly!" << std::endl;
        }
	}
	
	std::string Name()const
	{
		std::ostringstream oss;
		oss << _x << " " << _op << " " << _y << " = ? ";

		return oss.str();
	}

	std::string operator()()const
	{
		std::optional<T> opRet = _TaskFunc(_x, _y, _op);
		//std::cout << "operator()" << std::endl;
		std::ostringstream oss;
		if(opRet.has_value()) {oss << _x << " " << _op << " " << _y << " = " << opRet.value();}
		else {oss << "ERROR: /0";}
		return oss.str();
	}
private:
	T _x;
	T _y;
	char _op;// a + b, op = '+'
	TaskFunc_t _TaskFunc;
};
#endif