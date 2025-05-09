
#include "TypeInfo.h"

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

BEGIN_CLASS(Character)
FUCNTIONS(FUNC(&Character::GetName), FUNC(&Character::IsHit), FUNC(&Character::SetName));
END_CLASS()

int main() {
	auto typeInfo = TypeInfo<Character>();
	std::cout << std::get<0>(typeInfo.functions).name << std::endl;
}