#include <cstddef>
#include <cassert>
#include <algorithm>
#include <iostream>

const int default_capa = 10;

template <typename T>
class Vector
{
public:
	typedef T *iterator;

private:
	/* data */
	iterator _start;
	iterator _finish;
	iterator _endStorage;

private:
	void Allocate(); // 自动扩容

public:
	Vector(/* args */);
	Vector(const T &val);
	Vector(const Vector<T> &x);
	~Vector();

	iterator begin();
	iterator end();

	size_t Size() const;	 // 返回 size
	size_t Capacity() const; // 返回 capacity
	bool Empty() const;		 // 判断是否为空

	void Reserve(size_t n);				 // 容量增加至n
	void Resize(size_t n, const T &val); // size 调整为n, const T& val = T()，T 有不存在构造函数的可能

	const Vector<T> &operator=(const Vector<T> &x); // 赋值
	T &operator[](size_t pos);						// 访问 pos 位置元素，可修改
	const T &operator[](size_t pos) const;			// 访问 pos 位置元素，不可修改
	// std::ostream& operator<<(std::ostream os);

	iterator Erase(iterator pos);		// 删除 pos 位置元素
	void Erase(iterator pos, size_t n); // 删除 [pos, pos +n ) 范围元素
	void clear();						// 清空

	iterator Insert(iterator pos, const T &val);	   // pos 位置插入val
	void Insert(iterator pos, const T &val, size_t n); // pos 位置插入 n 个 val

	void PushBack(const T &val); // 尾插
	void PopBack();				 // 尾删

	void Swap(Vector<T> x); // 交换两个vector
};

template <typename T>
void Vector<T>::Allocate()
{
	int capa = (_start == nullptr ? default_capa : 2 * (_endStorage - _start));
	Reserve(capa);
}

template <typename T>
Vector<T>::Vector(/* args */)
	: _start(nullptr),
	  _finish(nullptr),
	  _endStorage(nullptr)
{
}

template <typename T>
Vector<T>::Vector(const T &val)
	: _start(nullptr),
	  _finish(nullptr),
	  _endStorage(nullptr)
{
	Allocate();
	*_start = val;
	++_finish;
}

template <typename T>
Vector<T>::Vector(const Vector<T> &x)
{
	size_t xCapa = x.Capacity();
	size_t xSize = x.Size();

	_start = new T[xCapa];

	for (size_t i = 0; i < xSize; ++i)
		*(_start + i) = *(x._start + i);

	_finish = _start + xSize;
	_endStorage = _start + xCapa;
}

template <typename T>
Vector<T>::~Vector()
{
	delete[] _start;
	_start = nullptr;
	_finish = nullptr;
	_endStorage = nullptr;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin()
{
	return _start;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end()
{
	return _finish;
}

template <typename T>
size_t Vector<T>::Size() const
{
	return _finish - _start;
}

template <typename T>
size_t Vector<T>::Capacity() const
{
	return _endStorage - _start;
}

template <typename T>
bool Vector<T>::Empty() const
{
	return _finish == _start;
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{
	if (_start == nullptr)
	{
		_start = new T[n];
		_finish = _start;
		_endStorage = _start + n;
	}
	else
	{
		T *newStart = new T[n];
		size_t curSize = Size();

		// 在类的内部，直接使用指针往往效率更高
		for (size_t i = 0; i < curSize; ++i)
		{
			newStart[i] = _start[i];
		}

		// size_t i = 0;
		//  for (const auto &v : *this)
		//  {
		//  	newStart[i++] = v;
		//  }

		delete[] _start;
		_start = newStart;
		_finish = _start + curSize;
		_endStorage = _start + n;
	}
}

template <typename T>
void Vector<T>::Resize(size_t n, const T &val)
{
	if (n < Size())
		_finish = _start + n;
	else
	{
		if (n > Capacity())
			Reserve(n);
		size_t increment = n - Size();
		while (increment)
		{
			PushBack(val);
			--increment;
			//++_finish;
		}
	}
}

template <typename T>
const Vector<T> &Vector<T>::operator=(const Vector<T> &x)
{
	if (_start != x._start)
	{
		delete[] _start;

		size_t n = x.Capacity();
		_start = new T[n];

		for (const auto &v : x)
		{
			PushBack(v);
		}
		_finish = x._finish;
		_endStorage = x._endStorage;
	}
}

template <typename T>
T &Vector<T>::operator[](size_t pos)
{
	assert(pos < Size());
	return *(_start + pos);
}

template <typename T>
const T &Vector<T>::operator[](size_t pos) const
{
	assert(pos < Size());
	return *(_start + pos);
}

template <typename T>
typename Vector<T>::iterator Vector<T>::Erase(iterator pos)
{
	iterator endPos = end();
	assert(pos != endPos);
	size_t oldPos = pos - begin();

	for (iterator it = pos; it != endPos - 1; ++it)
	{
		*it = *(it + 1);
	}
	--_finish;

	return _start + oldPos;
}

template <typename T>
void Vector<T>::Erase(iterator pos, size_t n)
{
	iterator endPos = end();
	assert(pos != endPos);
	if (pos + n > endPos)
		_finish = pos;
	else
	{
		iterator it = pos;
		for (int i = n; i > 0; --i, ++it)
		{
			*it = *(it + n);
		}

		_finish -= n;
	}
}

template <typename T>
void Vector<T>::clear()
{
	_finish = _start;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::Insert(iterator pos, const T &val)
{
	if (_finish == _endStorage)
	{
		size_t oldsize = _finish - _start;
		Allocate();
		pos = begin() + oldsize;
	}
	++_finish;
	for (iterator it = end() - 1; it != pos; --it)
		*it = *(it - 1);

	*pos = val;

	return pos;
}

template <typename T>
void Vector<T>::Insert(iterator pos, const T &val, size_t n)
{
	if ((size_t)(_endStorage - _finish) < n)
	{
		size_t nPos = pos - begin();
		Reserve(Size() + n);
		pos = begin() + nPos;
	}

	_finish += n;
	for (iterator it = end() - 1; it != pos + n - 1; --it)
	{
		*it = *(it - n);
	}

	while (n)
		*(pos + --n) = val;
}

template <typename T>
void Vector<T>::PushBack(const T &val)
{
	Insert(end(), val);
}

template <typename T>
void Vector<T>::PopBack()
{
	Erase(end() - 1);
}

template <typename T>
void Vector<T>::Swap(Vector<T> x)
{
	std::swap(_start, x._start);
	std::swap(_finish, x._finish);
	std::swap(_endStorage, x._endStorage);
}

// 因为没有定义 const_iterator，导致没有定义const begin() 和 end()
// 如果 const Vector<T>& x，会导致在范围 for 中调用 const begin() 和 end() 时出错
template <typename T>
std::ostream &operator<<(std::ostream &out, Vector<T> &x)
{
	for (const auto &v : x)
	{
		out << v << " ";
	}
	return out;
}
