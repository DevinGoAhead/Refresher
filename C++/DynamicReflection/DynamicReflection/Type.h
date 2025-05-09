#pragma once

#include <string>
#include <type_traits>
#include <vector>


class EnumFactory;

class Type {
private:
	friend class EnumFactory;
public:
	virtual ~Type() = default;
	Type(const std::string name) : _name(name){}
	const std::string& GetName() const { return _name; }
private:
	std::string _name; // 类型名称
}; // end of Type

class Numbric : public Type{
public:
	enum class Kind : int8_t{
		Int8, Int16, Int32, Int64, Int128,
		Float, Double
	};
public:
	Numbric(Kind kind, bool isSinged) : Type(GetName(kind)), _kind(kind), _isSigned(isSinged){}
	Kind GetKind() const { return _kind; }
	bool IsSigned() const { return _isSigned; }
	template <typename T>
	static Numbric Create() {return Numbric{ KindDetect<T>(), std::is_signed_v<T> }; } // 一个静态对象

private:
	static std::string GetName(Kind kind) {
		switch (kind) {
		case Kind::Int8:
			return "Int8";
		case Kind::Int16:
			return "Int16";
		case Kind::Int32:
			return "Int32";
		case Kind::Int64:
			return "Int64";
		case Kind::Int128:
			return "Int128";
		case Kind::Float:
			return "Float";
		case Kind::Double:
			return "Double";
		}
		return "UnknowType";
	}

	template <typename T>
	Kind KindDetect() {
		if constexpr (std::is_same_v<T, char>) return Kind::Int8;
		else if constexpr (std::is_same_v<T, short>) return Kind::Int16;
		else if constexpr (std::is_same_v<T, int>) return Kind::Int32;
		else if constexpr (std::is_same_v<T, long>) return Kind::Int64;
		else if constexpr (std::is_same_v<T, long long>) return Kind::Int128;
		else if constexpr (std::is_same_v<T, float>) return Kind::Float;
		else if constexpr (std::is_same_v<T, double>) return Kind::Double;
	}
private:
	bool _isSigned;
	Kind _kind;
}; // end of Numberic

class Enum : public Type {
public:
	Enum() : Type("Unknow_enum") {} //默认构造
	using EValueType = int;
	struct Item {
		std::string _name; // 枚举类型名称
		EValueType _value;// 枚举项的值
	};

	void Add(std::string name, EValueType value) { _items.emplace_back(name, value); }
	const auto& GetItems() const { return _items; }
private:
	std::vector<Item> _items;
}; // end of Enum

template<typename T>
class NumbricFactory final {
public:
	NumbricFactory(const NumbricFactory&) = delete;
	NumbricFactory(NumbricFactory&) = delete;
	//...
	
	static NumbricFactory* GetInstance() {
		if (!_instance) {
			_instance = new NumbricFactory();
		}
		return _instance;
	}
	auto Info() const { return _info; }
private:
	NumbricFactory() {
		_info = Numbric::Create<T>();
	}
private:
	Numbric _info;
	static NumbricFactory* _instance;
};
template<typename T>
NumbricFactory<T>* NumbricFactory<T>::_instance = nullptr; // 类外初始化
// end of NumbricFactory

class EnumFactory final {
public:
	EnumFactory(const EnumFactory&) = delete;
	EnumFactory(EnumFactory&) = delete;
	//...

	static EnumFactory* GetInstance() {
		if (!_instance) {
			_instance = new EnumFactory();
		}
		return _instance;
	}

	//注册枚举类型名称
	void Register(const std::string& name) {
		_info._name = name;
	}
	template<typename EMType>
	void Add(const std::string& name, EMType value) {
		_info.Add(name, static_cast<int>(value));
	}
	auto Info() const { return _info; }
private:
	EnumFactory() {
		_info = Enum(); // 默认构造, 枚举类型名: "Unknow_enum"
	}
private:
	Enum _info;
	static EnumFactory* _instance;
};

EnumFactory* EnumFactory::_instance = nullptr; // 类外初始化
// end of EnumFactory

template<typename T>
class Factory final {
public:
	static auto GetFactory() {
		if constexpr (std::is_fundamental_v<T>) { return NumbricFactory<T>::GetInstance(); }
		else if constexpr (std::is_enum_v<T>) { return EnumFactory::GetInstance(); }
	}
};

template<typename T>
auto Register() {
	return Factory<T>::GetFactory();
}



