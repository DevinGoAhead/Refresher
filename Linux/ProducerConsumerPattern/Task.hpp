// 这里更多的目的是为了练习使用c++的一些新特性, 实际不用写这么复杂
// 继承与多态也不是很有必要
#ifndef __Task
#define __Task

#include "Global.h"

#include <sstream>
#include <functional>

// R 返回值类型, F 函数类型, Args 参数模板包
template<typename R, typename F, typename... Args>
class Task
{
public:
	using TaskFunc_t = std::function<R(Args...)>;
public:
	Task() = default;
	Task(F&& Func, Args&&... args) 
	: _TaskFunc([TaskFunc = std::forward<F>(Func), ... argsCaptured = std::forward<Args>(args)](Args&&... fargs) -> R{return TaskFunc(argsCaptured...);})
	, _tuple(std::forward<Args>(args)...)
	{}
	
	virtual std::string Name() = 0;
	virtual std::string operator()() = 0;
	virtual ~Task() = default;
protected:
	TaskFunc_t _TaskFunc;
	std::tuple<Args...> _tuple;
};

// 计算: 从原始任务缓冲队列中获取任务并进行计算, 将计算结果返回到外部
// R 返回值类型, F 函数类型, Args 参数模板包
template<typename R, typename F, typename... Args>
class CalTask : public Task<R, F, Args...>
{
public:
	//using TaskFunc_t = std::function<std::optional<T>(T, T, char)>;
public:
	CalTask() = default;
	CalTask(F&& Func, Args&&... args) : Task<R, F, Args...>(std::forward<F>(Func), std::forward<Args>(args)...)
	{}
	
	virtual std::string Name() override
	{
		return std::apply([this](int x, int y, char op) -> std::string {
			std::ostringstream oss;
			oss << x << " " << op << " " << y << " = ? ";
			return oss.str();
		},this->_tuple);
	}

	virtual std::string operator()()override
	{
		return std::apply([this](int x, int y, char op) -> std::string {
			std::ostringstream oss;
			std::optional<int> opRet = this->_TaskFunc(x, y, op);
			
			if(opRet.has_value()) {oss << x << " " << op << " " << y << " = " << opRet.value();}
			else {oss << "ERROR: /0";}
			return oss.str(); 
		},this->_tuple);
	}
	virtual ~CalTask() override
	{}
private:
};

// 存储: 从计算结果缓冲队列中获取内容, 将内容存储到文件
// R 返回值类型, F 函数类型, Args 参数模板包
template<typename R, typename F, typename... Args>
class SaveTask : public Task<R, F, Args...>
{
public:
	//using TaskFunc_t = std::function<std::optional<T>(T, T, char)>;
public:
	SaveTask() = default;
	SaveTask(F&& Func, Args&&... args) : Task<R, F, Args...>(std::forward<F>(Func), std::forward<Args>(args)...)
	{}
	
	virtual std::string Name() override
	{
		return std::get<1>(this->_tuple);
	}

	virtual std::string operator()()override
	{
		auto pathName = std::get<0>(this->_tuple);
		std::cout << pathName << std::endl;
		auto ret = std::get<1>(this->_tuple);
		std::cout << ret << std::endl;
		this->_TaskFunc(pathName, ret);
		std::string msg = ret + "to" + pathName;
		return msg;
	}
	virtual ~SaveTask() override
	{}
private:
};

#endif