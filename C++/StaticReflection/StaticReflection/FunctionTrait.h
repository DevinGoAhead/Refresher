#pragma once

#include <type_traits>
#include <tuple>

// ���Ժ�������
//int Foo(float a) { return 1; }
//using F = int(float);
//// ���� Foo ���� ָ��, ����Ӧ����  F* f = Foo
//// int(float) ���Ա�ʶ��Ϊ��������(��Ժ���ָ������), ��Ҳ������ʶ������, �޷����������Ͷ������ 
//// F f = Foo; 

// �������Ա��������
// �Ǿ�̬��Ա����ֻ��ָ������, û����ȫ�ֺ��������ĺ�������
// ��̬��Ա����������ָ�����͵�
//class Actor {
//public:
//	void SetName(char name) {};
//	char GetName() const { return 'a'; };
//};
//
//auto fc = Actor::SetName;

namespace Detail {
	template <typename F>
	struct BasicFunctionTraits {};

	template <typename Ret, typename... Args>
	struct BasicFunctionTraits<Ret(Args...)> {
		using argsType = std::tuple<Args...>;
		using retType = Ret;
	};
}

template <typename F>
struct FunctionTraits {};

// ȫ�ֺ����ػ�
// Ret(Args...), ��������, �뺯��ָ���������Ӧ
// Ret(Args...), ������ʶ������, �޷����������Ͷ������ 
template <typename Ret, typename... Args>
struct FunctionTraits<Ret(Args...)> : public Detail::BasicFunctionTraits<Ret(Args...)> {
	using funcType = Ret(Args...); 
	using funcPointerType = Ret(*)(Args...);
	using argsTypeWithClass = std::tuple<Args...>; // ����μ� ���Ա�����ػ�
	// static ȷ������Ҫʵ�������ܷ���
	// constexpr �����ھ�����ȷ
	static constexpr bool isMember = false;
	static constexpr bool isConst = false;
};

template <typename Ret, typename... Args>
struct FunctionTraits<Ret(*)(Args...)> : public FunctionTraits<Ret(Args...)> {};

// ���Ա�����ػ�
// Ret(Class::*)(Args...), �Ǿ�̬��Ա����ֻ��ָ������, ��û�к�������
template <typename Class, typename Ret, typename... Args>
struct FunctionTraits<Ret(Class::*)(Args...)> : public Detail::BasicFunctionTraits<Ret(Args...)> {
	using funcType = Ret(Class::*)(Args...);
	using funcPointerType = Ret(Class::*)(Args...);
	using argsTypeWithClass = std::tuple<Class*, Args...>; // Class* ָ���ǳ�Ա���������ص� this ָ��
	// static ȷ������Ҫʵ�������ܷ���
	// constexpr �����ھ�����ȷ
	static constexpr bool isMember = true;
	static constexpr bool isConst = false;
};

// const ���Ա�����ػ�
// Ret(Class::*)(Args...), �Ǿ�̬��Ա����ֻ��ָ������, ��û�к�������
template <typename Class, typename Ret, typename... Args>
struct FunctionTraits<Ret(Class::*)(Args...) const> : public Detail::BasicFunctionTraits<Ret(Args...) const> {
	using funcType = Ret(Class::*)(Args...) const;
	using funcPointerType = Ret(Class::*)(Args...) const;
	using argsTypeWithClass = std::tuple<Class*, Args...>; // Class* ָ���ǳ�Ա���������ص� this ָ��
	// static ȷ������Ҫʵ�������ܷ���
	// constexpr �����ھ�����ȷ
	static constexpr bool isMember = true;
	static constexpr bool isConst = true;
};