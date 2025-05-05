
#include "FieldTrait.h"

#include <iostream>
#include <string>

/******************************************************************************************************/
std::string Foo_1(float args1) { return "abc"; }

template<typename T>
auto Foo_2(T) -> int = delete;

class Character {
public:
	void SetName(std::string name) { _name = name; };
	std::string GetName() const { return "name"; };
	bool IsHit(const Character& otherCharactor) {
		return true;
	}
private:
	std::string _name;
	bool _isMale;
};

/******************************************************************************************************/

template <typename Ret, typename... Args>
auto FunctionPointerType(Ret(*)(Args...)) -> Ret(*)(Args...); // ��������, ����ֵ������������ͱ�����ͬ

template <typename Class, typename Ret, typename... Args>
auto FunctionPointerType(Ret(Class::*)(Args...)) -> Ret(Class::*)(Args...);

template <typename Class, typename Ret, typename... Args>
auto FunctionPointerType(Ret(Class::*)(Args...) const) -> Ret(Class::*)(Args...);

// FunctionPointerType ��һ������, ȷ�е�˵�Ǻ�������
// auto F, F ��ֵ, auto ���Ƶ�ȷ��ֵ������, ������ F ����һ��ֵ
// FunctionPointerType(F), ��һ����������, ����ֻ��һ������, ���Կ�ʡ��<>,��ʽʵ����
// FunctionPointerType(F) ��Ȼ�﷨���ǵ���, ������ decltype �в�δ�����ִ��, ֻ�ǲ������������
// FunctionPointType_t ��ʱ��һ������, ģ������
template <auto F>
using FunctionPointType_t = decltype(FunctionPointerType(F));
// using FunctionPointType_t = decltype(F); // ������ֱ������Ҳ����?


template <typename T>
struct TypeInfo {};

int main() {
	//auto field = FieldTraits<decltype(&Character::GetName)>{ &Character::GetName };
	//auto field = FieldTraits{ &Character::GetName };
	auto field = FieldTraits{ &Foo_1 };
	std::cout << field.IsMember() << std::endl;
	std::cout << field.IsConst() << std::endl;
	std::cout << field.pointer << std::endl;

	std::cout << &Character::GetName << std::endl;
	std::cout << &Character::SetName << std::endl;
	std::cout << &Character::IsHit << std::endl;
}