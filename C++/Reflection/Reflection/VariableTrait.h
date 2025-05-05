#pragma once

#include <string>
#include <type_traits>
#include <tuple>

// 1. trait T from T*
template<typename T>
struct RemovePointer {};

template<typename T>
struct RemovePointer<T*> {
	using type = T;
};

// template parameters
template<typename... Args>
struct RemovePointer<Args*...> {
	using type = std::tuple<Args...>;
};

// 2. trait T from const T
template<typename T>
struct RemoveConst {};

template<typename T>
struct RemoveConst<const T> {
	using type = T;
};

// template parameters
template<typename... Args>
struct RemovePointer<const Args...> {
	using type = std::tuple<Args...>;
};

// 3. trait return Type, args Type from function
template<typename T>
struct FunctionTrait {};

// global function, single parameter
template<typename Ret, typename Args>
struct FunctionTrait<Ret(*)(Args)> {
	using retType = Ret;
	using argsType = Args;
};

// global function, none parameters
template<typename Ret>
struct FunctionTrait<Ret(*)()> {
	using retType = Ret;
};

// global function, template parameters
template<typename Ret, typename... Args>
struct FunctionTrait<Ret(*)(Args...)> {
	using retType = Ret;
	using argsType = std::tuple<Args...>;
};

// class member function, single parameter
template<typename Class, typename Ret, typename Args>
struct FunctionTrait<Ret(Class::*)(Args)> {
	using classType = Class;
	using retType = Ret;
	using argsType = Args;
};

// class member function, single parameter, const
template<typename Class, typename Ret, typename Args>
struct FunctionTrait<Ret(Class::*)(Args) const> {
	using classType = Class;
	using retType = Ret;
	using argsType = Args;
};

// class member function, none parameters
template<typename Class, typename Ret>
struct FunctionTrait<Ret(Class::*)()> {
	using classType = Class;
	using retType = Ret;
};

// class member function, none parameters, const
template<typename Class, typename Ret>
struct FunctionTrait<Ret(Class::*)() const> {
	using classType = Class;
	using retType = Ret;
};

// class member function, template parameters
template<typename Class, typename Ret, typename... Args>
struct FunctionTrait<Ret(Class::*)(Args...)> {
	using classType = Class;
	using retType = Ret;
	using argsType = std::tuple<Args...>;
	bool isMemberFunction = true;
};

