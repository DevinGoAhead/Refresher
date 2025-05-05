#pragma once

#include "FunctionTrait.h"
#include "VariableTrait.h"

namespace Detail {
	template <typename T>
	struct IsFunction {
		static constexpr bool value = 
			// ��Ϊstd::is_function �Ὣ ȫ�ֺ���ָ��ʶ��Ϊ ָ��, �����Ǻ���, ���ȥ��ָ�뻹ԭΪ��������
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
// VariableTraits �� FunctionTraits �����������
// ������ VariableTraits �� FunctionTraits �Ļ����ϱ��溯���������ָ��
template <typename T>
struct FieldTraits : public Internal::BasicFieldTraits<T, IsFunction_v<T>> {
	constexpr FieldTraits(T&& ptr) : pointer(ptr) {}
	T pointer;
};