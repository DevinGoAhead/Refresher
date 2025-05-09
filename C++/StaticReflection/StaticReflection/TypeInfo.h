#pragma once
#include "FieldTrait.h"
template<typename T>
struct TypeInfo;

// ����Ҫ���ľ��ǽ��������ָ�뱣����class TypeInfo �ľ�̬��Ա functions ��
#define BEGIN_CLASS(X) \
	template<>\
	struct TypeInfo<X> {
#define FUCNTIONS(...) static constexpr auto functions = std::make_tuple(__VA_ARGS__);/* ... Ϊ��ͬ���͵ĺ���ָ��*/
#define FUNC(F) FieldTraits{F, #F}/*��ʽʵ����, ʡ��<F>, ���� FieldTraits ��ʱ����, ������һ��ָ��*/
#define END_CLASS() };

// ģ�庯��, �����Ƿ���һ�� �ػ����͵� TypeInfo ����
// ��Ϊ functions �� TypeInfo<X>�ľ�̬��Ա����, ��������Ӧ�ػ����� TypeInfo ���͵Ķ���, ָ�붼���Է��ʵ�TypeInfo<X>::functions
// ���������Ƿ�����б�Ҫ��?
// �󾭹��Ҳ���, �Ҿ���������ȫû��Ҫ
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