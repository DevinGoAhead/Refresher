#pragma once

//#include <string>
#include <type_traits>
#include <tuple>

//class Test {
//public:
//	int a;
//};
//using p = decltype(&Test::a); // ����һ�����Ա������ָ�����͵���ʽ, ���Խ��Ϊ int Test::*

//Detail ��ʾ��Щ���ݲ�Ӧ���ⲿʹ��
namespace Detail {
	//��ͨ����
	template <typename T>
	struct VariableType {
		using type = T;
	};
	//�ػ�: ���Ա����ָ������, trait ����Ա��������
	template <typename Class, typename T>
	struct VariableType <T Class::*> {
		using type = T;
	};
}

// Detail::VariableType �� "��Ա" type �ı���
template <typename T>
using VariableType_t = typename Detail::VariableType<T>::type;

namespace Internal {
	template <typename T>
	struct BasicVariableTrait {
		using type = VariableType_t<T>; // ���������޷��ж��ǳ�Ա����������ͨ����
		static constexpr bool isMember = std::is_member_pointer_v<T>; //�� std::is_member_pointer<T>::value;
	};
}

//BasicVariableType �ǽ�δ�ػ����ػ��� VariableType �ۺϵ�һ��
// ������Ҫ��Բ�ͬ���Ͷ�  BasicVariableType �����ػ�

template<typename T>
struct VariableTraits {};

// ����ļ̳н����ǳ�Ա�ϵļ̳�, ��û���߼��ϵ� is-a �Ĺ�ϵ
template<typename T>
struct VariableTraits<T*> : public Internal::BasicVariableTrait<T> {
	using pointerType = T*;
};

template<typename Class, typename T>
struct VariableTraits<T Class::*> : public Internal::BasicVariableTrait<T> {
	using pointerType = T Class::*;
	using classType = Class;

};

// ���յ�Ŀ�ľ��ǽ���ͨ��������, ��ͨ����ָ������, ���Ա��������, ���Ա��������, ������, �ۺϵ���һ���ṹ����
// ����Ҫ����һ������ʹ�ò�ͬ�Ľṹ��
