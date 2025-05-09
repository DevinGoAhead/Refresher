#pragma once
#include "FieldTrait.h"
template<typename T>
struct TypeInfo;

// 这里要做的就是将多个函数指针保存在class TypeInfo 的静态成员 functions 中
#define BEGIN_CLASS(X) \
	template<>\
	struct TypeInfo<X> {
#define FUCNTIONS(...) static constexpr auto functions = std::make_tuple(__VA_ARGS__);/* ... 为不同类型的函数指针*/
#define FUNC(F) FieldTraits{F, #F}/*隐式实例化, 省略<F>, 构造 FieldTraits 临时对象, 本质是一个指针*/
#define END_CLASS() };

// 模板函数, 本意是返回一个 特化类型的 TypeInfo 对象
// 因为 functions 是 TypeInfo<X>的静态成员对象, 因此任意对应特化类型 TypeInfo 类型的对象, 指针都可以访问到TypeInfo<X>::functions
// 所以这里是否真的有必要呢?
// 后经过我测试, 我觉得这里完全没必要
template<typename T>
auto TypeInfoF() {
	return TypeInfo<T>{};
}
//template<T>
//void VisitTuple(std::tuple<Args...> tuple, Class*)template<size_t index, typename... Args>
//void VisitTuple(std::tuple<Args...> tuple, Class*)

template<size_t index, typename... Args, typename Class>
void VisitTuple(std::tuple<Args...> tuple, Class* instance) {
	using tupleType = std::tuple<Args...>;
	if constexpr (index >= std::tuple_size_v<tupleType>) return;
	else {
		if constexpr (auto elem = std::get<index>(tuple); elem.argsCount())
	}
}