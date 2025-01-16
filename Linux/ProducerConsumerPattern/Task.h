#ifndef __Task
#define __Task
#include "Global.h"

//#include <tuple>
#include <sstream>

// 计算: 从原始任务缓冲队列中获取任务并进行计算
// 存储: 将计算完成的任务放到存储计算结果的缓冲队列

template<typename F, typename... Args>
class Calculator
{
public:
	using return_t = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;
public:
	Calculator(F&& func, Args&&... args)
	{
		auto task = std::make_shared<std::function<return_t()>>
		(
			[TaskFunc = std::forward<F>(func), ... argsCaptured = std::forward<Args>(args)]() -> return_t
			{return TaskFunc(argsCaptured...);}
			
		);
		std::ostringstream oss;
		((oss << args << " "),...);
		oss << "= ?";
		_name = oss.str();

	}
	//operator()
	const std::string& Name() {return _name;}
	
private:
	std::string _name;
	char _op;

};

#endif