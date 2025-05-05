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

// 2. trait T from const T
template<typename T>
struct RemoveConst {};

template<typename T>
struct RemoveConst<const T> {
	using type = T;
};

// 3. trait return Type, args Type from function
template<typename T>
struct FunctionTrait {};

// global function
template<typename Ret, typename Args>
struct FunctionTrait<Ret(*)(Args)> {
	using retType = Ret;
	using argsType = Args;
};

// class member function
template<typename Class, typename Ret, typename Args>
struct FunctionTrait<Ret(Class::*)(Args)> {
	using classType = Class;
	using retType = Ret;
	using argsType = Args;
};


