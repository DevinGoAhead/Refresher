#pragma once

#include <type_traits>
#include <tuple>

// 测试函数类型
//int Foo(float a) { return 1; }
//using F = int(float);
//// 这里 Foo 该是 指针, 所以应该是  F* f = Foo
//// int(float) 可以被识别为函数类型(相对函数指针类型), 但也仅限于识别类型, 无法用于声明和定义变量 
//// F f = Foo; 

// 测试类成员函数类型
// 非静态成员函数只有指针类型, 没有像全局函数那样的函数类型
// 静态成员函数则是有指针类型的
//class Actor {
//public:
//	void SetName(char name) {};
//	char GetName() const { return 'a'; };
//};
//
//auto fc = Actor::SetName;

namespace Detail {
	template <typename F>
	struct BasicFunctionTraits {};

	template <typename Ret, typename... Args>
	struct BasicFunctionTraits<Ret(Args...)> {
		using argsType = std::tuple<Args...>;
		using retType = Ret;
	};
}

template <typename F>
struct FunctionTraits {};

// 全局函数特化
// Ret(Args...), 函数类型, 与函数指针类型相对应
// Ret(Args...), 仅限于识别类型, 无法用于声明和定义变量 
template <typename Ret, typename... Args>
struct FunctionTraits<Ret(Args...)> : public Detail::BasicFunctionTraits<Ret(Args...)> {
	using funcType = Ret(Args...); 
	using funcPointerType = Ret(*)(Args...);
	using argsTypeWithClass = std::tuple<Args...>; // 含义参见 类成员函数特化
	// static 确保不需要实例化就能访问
	// constexpr 编译期就能明确
	static constexpr bool isMember = false;
	static constexpr bool isConst = false;
};

template <typename Ret, typename... Args>
struct FunctionTraits<Ret(*)(Args...)> : public FunctionTraits<Ret(Args...)> {};

// 类成员函数特化
// Ret(Class::*)(Args...), 非静态成员函数只有指针类型, 而没有函数类型
template <typename Class, typename Ret, typename... Args>
struct FunctionTraits<Ret(Class::*)(Args...)> : public Detail::BasicFunctionTraits<Ret(Args...)> {
	using funcType = Ret(Class::*)(Args...);
	using funcPointerType = Ret(Class::*)(Args...);
	using argsTypeWithClass = std::tuple<Class*, Args...>; // Class* 指的是成员函数中隐藏的 this 指针
	// static 确保不需要实例化就能访问
	// constexpr 编译期就能明确
	static constexpr bool isMember = true;
	static constexpr bool isConst = false;
};

// const 类成员函数特化
// Ret(Class::*)(Args...), 非静态成员函数只有指针类型, 而没有函数类型
template <typename Class, typename Ret, typename... Args>
struct FunctionTraits<Ret(Class::*)(Args...) const> : public Detail::BasicFunctionTraits<Ret(Args...) const> {
	using funcType = Ret(Class::*)(Args...) const;
	using funcPointerType = Ret(Class::*)(Args...) const;
	using argsTypeWithClass = std::tuple<Class*, Args...>; // Class* 指的是成员函数中隐藏的 this 指针
	// static 确保不需要实例化就能访问
	// constexpr 编译期就能明确
	static constexpr bool isMember = true;
	static constexpr bool isConst = true;
};