#include <cassert>
#include <cstring>
#include <iostream>

const size_t defaultCpct = 10;
const size_t nPos = -1;
typedef char *iterator;//String 部分迭代器仅示意，下面的定义也并不合理，List 部分会深入定义

class String
{
private:
	void Allocate()
	{
		_capacity = _capacity * 2;
		Reserve(_capacity);
	}

public:
	iterator begin() const
	{
		return _pStr;
	}

	iterator end() const
	{
		return _pStr + _size;
	}

	const char *C_Str(const String str) const // 通过 String 返回一个 C-String
	{
		return _pStr;
	}

	String() // 构造空字符串
	{
		_capacity = defaultCpct;
		_size = 0;
		_pStr = new char[_capacity + 1];
		*_pStr = '0';
	}

	String(const char *cStr) // 通过 C-String 拷贝构造
	{
		assert(cStr); // cStr != nullptr

		size_t len = strlen(cStr);
		_size = len;
		_capacity = (len == 0) ? defaultCpct : len;

		_pStr = new char[_capacity + 1];
		strcpy(_pStr, cStr);
	}

	String(const String &str) // 通过 String 拷贝构造
	{
		_size = str._size;
		_capacity = str._capacity;

		_pStr = new char[_capacity + 1];
		strcpy(_pStr, str._pStr);
	}

	const String &operator=(const char *cStr) // 通过 C-String 赋值
	{
		assert(cStr); // cStr != nullptr
		size_t len = strlen(cStr);
		_size = len;
		if (_capacity < len)
		{
			_capacity = len;
			_pStr = new char[_capacity + 1];
		}

		strcpy(_pStr, cStr);
		return *this;
	}

	const String &operator=(const String &str) // 通过 String 赋值
	{
		_size = str._size;
		if (_capacity < str._capacity)
		{
			_capacity = str._capacity;
			_pStr = new char[_capacity + 1];
		}

		strcpy(_pStr, str._pStr);
		return *this;
	}

	const char& operator[](size_t pos) const // 访问第 pos（起始值为0）个字符，不允许修改
	{
		assert(pos < _size);
		return *(_pStr + pos);
	}

	char& operator[](size_t pos) // 访问第 pos（起始值为0）个字符，允许修改
	{
		assert(pos < _size);
		return *(_pStr + pos);
	}

	const size_t Size() const // 获取字符串的长度
	{
		return _size;
	}

	const size_t Capacity() const // 获取字符串的容量，允许的最大长度
	{
		return _capacity;
	}

	bool Empty() const // 检擦字符串是否为空，空-true，非空-false
	{
		return _size == 0;
	}

	void Reserve(size_t n) // 扩容至n, 不影响字符串长度和内容
	{
		if (_capacity < n)
		{
			String strTemp(_pStr);
			_capacity = n;
			_pStr = new char[_capacity + 1];
			strcpy(_pStr, strTemp._pStr);
		}
	}

	void Resize(size_t n, const char c) // 字符串长度调整为n，并以字符 c 填充
	{
		if (_capacity < n)
			Reserve(n);
		size_t cur = _size;
		_size = n;

		while (cur < _size)
		{
			(*this)[cur++] = c;
		}
		*(_pStr + cur) = '\0';
	}

	void Erase(size_t pos = 0, size_t len = nPos) // 删除 [pos, pos + len) 范围内的字符(pos 起始值为 0)
	{
		if (len == nPos || len + pos > _size) // 若未给定 len 或 pos + len > 字符串长度，则删除 pos 至尾端的所有字符
		{
			(*this)[pos] = '\0';
			_size = pos + 1;
		}
		else
		{
			size_t dePos = pos + len;
			size_t cur = pos;
			while (dePos < _size)
			{
				(*this)[cur++] = (*this)[dePos++];
			}
			(*this)[cur] = '\0';
			_size -= len;
		}
	}

	void Clear() // 清除字符串所有内容，字符串长度 归零
	{
		*_pStr = '\0';
		_size = 0;
	}

	void Insert(size_t pos, const char c) // 在 pos 位置插入字符 c 变量表示的字符
	{
		assert(pos <= _size);
		if (_size == _capacity)
			Allocate();
		
		size_t cur = _size;
		++_size;
		
		while (cur > pos)
		{
			//(*this)[cur--] = (*this)[cur];
			(*this)[cur] = (*this)[cur - 1]; // 添加花括号并调整索引
			--cur;
		}
		*(_pStr + cur) = c;
		*(_pStr + _size) = '\0';
	}

	String &Insert(size_t pos, const char *cStr) // 在第 pos（起始值为0）个字符的位置插入 C-String
	{
		assert(pos <= _size);
		size_t len = strlen(cStr);
		if (len != 0)
		{
			if (_capacity < len + _size)
				Reserve(len + _size);
		
			_size += len;
			size_t cur = _size - 1;
			size_t des = pos + len;
			*(_pStr + _size) = '\0';

			while(cur >= des)
			{
				(*this)[cur] = (*this)[cur - len];
				--cur;
				//std::cout << _pStr<<std::endl;
			}
			//std::cout << _pStr<<std::endl;

			//strcpy(_pStr + pos, cStr);//strcpy 复制完成后会在完成位置添加\0, 这将会截断字符串
			memcpy(_pStr + pos, cStr, len);
			//std::cout << _pStr<<std::endl;
		}

		return *this;
	}

	String &Insert(size_t pos, const String &str) // 在第 pos（起始值为0）个字符的位置插入 String
	{
		return Insert(pos, str._pStr);
	}

	void PushFront(char c) // 头插
	{
		Insert(0, c);
	}

	void PushBack(char c) // 尾插
	{
		Insert(_size, c);
	}

	char &Front(const char c) const // 返回头端字符
	{
		return *(_pStr);
	}

	char &Back(const char c) const // 返回尾端字符
	{
		return *(_pStr + _size - 1);
	}

	String &operator+=(const char c) // 尾端附加变量 c 表示的字符
	{
		Insert(_size, c);
		return *this;
	}

	String &operator+=(const char *cStr) // 尾端附加 C-String
	{
		Insert(_size, cStr);
		return *this;
	}

	String &operator+=(const String &str) // 尾端附加 String
	{
		Insert(_size, str._pStr);
		return *this;
	}

	size_t Find(const char c, size_t pos = 0)const // 自第 pos 个字符开始，匹配字符 c
	{
		assert(pos < _size);
		for(; pos < _size; ++pos)
		{
			if((*this)[pos] == c)
				return pos;
		}
		return nPos;//找不到返回 nPos（-1）
	}

	size_t Find(const char* cStr, size_t pos = 0)const // 自第 pos 个字符开始，匹配字符 C-String
	{
		assert(pos < _size);
		assert(cStr);
		char* des = strstr(_pStr + pos, cStr);
		if(des == nullptr)
			return des -_pStr;
		return des - _pStr;
	}

	size_t Find(const String& str, size_t pos = 0)const // 自第 pos 个字符开始，匹配字符 C-String
	{
		assert(pos < _size);
		char* des = strstr(_pStr + pos, str._pStr);
		if(des == nullptr)
			return des -_pStr;
		return des - _pStr;
	}
private:
	size_t _size;
	size_t _capacity;
	char *_pStr;
};

std::ostream& operator<<(std::ostream& os, const String& str)
{
	for(const auto& e : str)
	{
		os << e;
	}
	return os;
}