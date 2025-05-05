
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
auto FunctionPointerType(Ret(*)(Args...)) -> Ret(*)(Args...); // 函数声明, 返回值类型与参数类型保持相同

template <typename Class, typename Ret, typename... Args>
auto FunctionPointerType(Ret(Class::*)(Args...)) -> Ret(Class::*)(Args...);

template <typename Class, typename Ret, typename... Args>
auto FunctionPointerType(Ret(Class::*)(Args...) const) -> Ret(Class::*)(Args...);

// FunctionPointerType 是一个函数, 确切的说是函数声明
// auto F, F 是值, auto 可推导确定值的类型, 但本质 F 就是一个值
// FunctionPointerType(F), 是一个函数调用, 由于只有一个参数, 所以可省略<>,隐式实例化
// FunctionPointerType(F) 虽然语法上是调用, 但是在 decltype 中并未被真的执行, 只是参与了语义分析
// FunctionPointType_t 此时是一个类型, 模板类型
template <auto F>
using FunctionPointType_t = decltype(FunctionPointerType(F));
// using FunctionPointType_t = decltype(F); // 理论上直接这样也可以?


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