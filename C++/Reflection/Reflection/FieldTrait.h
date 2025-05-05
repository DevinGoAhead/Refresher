#pragma once

#include "FunctionTrait.h"
#include "VariableTrait.h"

namespace Detail {
	template <typename T>
	struct IsFunction {
		static constexpr bool value = 
			// 因为std::is_function 会将 全局函数指针识别为 指针, 而不是函数, 因此去掉指针还原为函数类型
			std::is_function_v<std::remove_pointer_t<T>> || std::is_member_function_pointer_v<T>;
	};
}

template <typename T>
constexpr bool IsFunction_v = Detail::IsFunction<T>::value;

namespace Internal {
	template <typename T, bool isFunction>
	struct BasicFieldTraits {};

	template <typename T>
	struct BasicFieldTraits<T, true> : public FunctionTraits<T> {
		using traitType = FunctionTraits<T>;
		constexpr bool IsMember() const { return traitType::isMember; }
		constexpr bool IsConst() const { return traitType::isConst; }
		constexpr bool IsFunction() const { return true; }
		constexpr bool IsVariable() const { return false; }
	};

	template <typename T>
	struct BasicFieldTraits<T, false> : public VariableTraits<T> {
		using traitType = VariableTraits<T>;
		constexpr bool IsMember() const { return traitType::isMember; }
		constexpr bool IsFunction() const { return false; }
		constexpr bool IsVariable() const { return true; }
	};
}
// VariableTraits 和 FunctionTraits 保存的是类型
// 这里在 VariableTraits 和 FunctionTraits 的基础上保存函数或变量的指针
template <typename T>
struct FieldTraits : public Internal::BasicFieldTraits<T, IsFunction_v<T>> {
	constexpr FieldTraits(T&& ptr) : pointer(ptr) {}
	T pointer;
};