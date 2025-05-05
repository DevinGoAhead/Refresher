
#include "VariableTrait.h"
#include <iostream>

std::string Foo(float args1) { return "abc"; }

class Charactor {
public:
	void SetName(std::string name) {};
	std::string GetName() const { return "name"; };
};

int main() {
	// trait from variable
	using typeNonPtr = RemovePointer<int*>::type;
	using typeNonConst = RemoveConst<const int>::type;

	// trait from global function
	std::string(*fPtr_a)(float) = Foo;
	using typeFunction_1 = FunctionTrait<decltype(fPtr_a)>;
	using typeFunction_2 = FunctionTrait<decltype(Foo)>;

	// trait from member function
	// 先了解成员函数指针的结构, 与全局函数的结构对比
	void(Charactor:: * fPtr_b)(std::string) = &Charactor::SetName;
	std::string(Charactor:: * fPtr_c)() const = &Charactor::GetName;

	using classType = FunctionTrait<decltype(fPtr_b)>::classType;
	using retType = FunctionTrait<decltype(fPtr_b)>::retType;
	using argsType = FunctionTrait<decltype(fPtr_b)>::argsType;

	std::cout << fPtr_b << std::endl;
}