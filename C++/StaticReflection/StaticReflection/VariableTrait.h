#pragma once

//#include <string>
#include <type_traits>
#include <tuple>

//class Test {
//public:
//	int a;
//};
//using p = decltype(&Test::a); // 测试一下类成员变量的指针类型的形式, 测试结果为 int Test::*

//Detail 表示这些内容不应被外部使用
namespace Detail {
	//普通变量
	template <typename T>
	struct VariableType {
		using type = T;
	};
	//特化: 类成员变量指针类型, trait 出成员变量类型
	template <typename Class, typename T>
	struct VariableType <T Class::*> {
		using type = T;
	};
}

// Detail::VariableType 的 "成员" type 的别名
template <typename T>
using VariableType_t = typename Detail::VariableType<T>::type;

namespace Internal {
	template <typename T>
	struct BasicVariableTrait {
		using type = VariableType_t<T>; // 单从这里无法判断是成员变量还是普通变量
		static constexpr bool isMember = std::is_member_pointer_v<T>; //或 std::is_member_pointer<T>::value;
	};
}

//BasicVariableType 是将未特化和特化的 VariableType 聚合到一起
// 现在又要针对不同类型对  BasicVariableType 重新特化

template<typename T>
struct VariableTraits {};

// 这里的继承仅仅是成员上的继承, 并没有逻辑上的 is-a 的关系
template<typename T>
struct VariableTraits<T*> : public Internal::BasicVariableTrait<T> {
	using pointerType = T*;
};

template<typename Class, typename T>
struct VariableTraits<T Class::*> : public Internal::BasicVariableTrait<T> {
	using pointerType = T Class::*;
	using classType = Class;

};

// 最终的目的就是将普通变量类型, 普通变量指针类型, 类成员变量类型, 类成员变量类型, 类类型, 聚合到了一个结构体中
// 不需要向上一版那样使用不同的结构体
