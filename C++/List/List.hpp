#include <cstddef>//定义了size_t
#include <iostream>
#include <algorithm>

namespace wxy
{
	// 节点结构
	template <typename T>
	struct __Node
	{
	public:
		__Node(T value = T())
			: _value(value)
			, _next(nullptr)
			, _prev(nullptr)
		{
		}

	public:
		T _value;
		__Node<T> *_next;
		__Node<T> *_prev;
	};

	template <typename T>
	class List;
	//iterator
	template<typename T, typename Ptr, typename Ref>
	class __Iterator
	{
		friend class List<T>;
	public:
		using Node = __Node<T>;
		using Self = __Iterator<T, Ptr, Ref>;
	public:
		__Iterator(Node* pNode)
		:_pNode(pNode)
		{}
		Ref operator*();
		Ptr operator->();
		Self operator++();
		Self operator++(int);
		Self operator--();
		Self operator--(int);
		bool operator==(const Self iter)const;
		bool operator!=(const Self iter)const;
	private:
		Node* _pNode;
	};
	
	template<typename T, typename Ptr, typename Ref>
	Ref __Iterator<T, Ptr, Ref>::operator*()
	{
		return _pNode->_value;
	}

	template<typename T, typename Ptr, typename Ref>
	Ptr __Iterator<T, Ptr, Ref>::operator->()
	{
		return &(_pNode->_value);
	}

	template<typename T, typename Ptr, typename Ref>
	typename __Iterator<T, Ptr, Ref>::Self __Iterator<T, Ptr, Ref>::operator++()
	{
		_pNode = _pNode->_next;
		return _pNode;
	}

	template<typename T, typename Ptr, typename Ref>
	typename __Iterator<T, Ptr, Ref>::Self __Iterator<T, Ptr, Ref>::operator++(int)
	{
		Node* cur = _pNode;
		_pNode = _pNode->_next;
		return cur;
	}

	template<typename T, typename Ptr, typename Ref>
	typename __Iterator<T, Ptr, Ref>::Self __Iterator<T, Ptr, Ref>::operator--()
	{
		_pNode = _pNode->_prev;
		return _pNode;
	}

	template<typename T, typename Ptr, typename Ref>
	typename __Iterator<T, Ptr, Ref>::Self __Iterator<T, Ptr, Ref>::operator--(int)
	{
		Node* cur = _pNode;
		_pNode = _pNode->_prev;
		return cur;
	}

	template<typename T, typename Ptr, typename Ref>
	bool __Iterator<T, Ptr, Ref>::operator==(const Self iter)const
	{
		return _pNode == iter._pNode;
	}

	template<typename T, typename Ptr, typename Ref>
	bool __Iterator<T, Ptr, Ref>::operator!=(const Self iter)const
	{
		return _pNode != iter._pNode;
	}

	//List, 带哨兵节点的环形链表
	template <typename T>
	class List
	{
	public:
		using Node = __Node<T>;
		using iterator = __Iterator<T, T*, T&>;

		// T 加不加const无所谓, 因为 三者 均只作为返回值类型
		// 当返回T(值返回)时,const 修饰是无意义的
		using const_iterator = __Iterator<T, const T*, const T&>;
	public:
		List();
		List(size_t n, const T &value = T());
		List(int n, const T &value = T());//避免n 对应的实参为int时，错误的调用 List(inputIterator first, inputIterator last);
		template<typename inputIterator>
		List(inputIterator first, inputIterator last);
		List(const List& x);
		List& operator=(const List x);
		iterator begin();
		const_iterator begin()const;
		iterator end();
		const_iterator end()const;
		iterator Insert(iterator pos, const T &value);
		iterator Erase(iterator pos);
		void Erase(iterator first, iterator last);
		void Clear();
		void PushFront(const T &value);
		void PushBack(const T &value);
		void PopFront();
		void PopBack();
		void Swap(List& x);
		bool Empty();
		size_t Size();
	private:
		Node* GetNode();//创建一个节点,value为默认值
		void EmptyInit();//创建含一个结点的链表
	private:
		Node* _pHead;
		size_t _size;
	};

	template <typename T>
	List<T>::List()
	{
		EmptyInit();
	}
	
	template <typename T>
	List<T>::List(size_t n, const T &value)// List<T>::List(size_t n, const T &value = T()) 这样会报错
	{
		EmptyInit();
		while(n--)
		{
			PushBack(value);
		}
	}

	template <typename T>
	List<T>::List(int n, const T &value)// List<T>::List(size_t n, const T &value = T()) 这样会报错
	{
		EmptyInit();
		while(n--)
		{
			PushBack(value);
		}
	}
	
	template <typename T>
	template<typename inputIterator>
	List<T>::List(inputIterator first, inputIterator last)
	{
		EmptyInit();
		while(first != last)
		{
			PushBack(*first);
			++first;
		}
	}
	template <typename T>
	List<T>::List(const List& x)
	{
		List<T> temp(x.begin(), x.end());
		EmptyInit();
		Swap(temp);

	}

	template <typename T>
	List<T>& List<T>::operator=(const List x)
	{
		EmptyInit();
		Swap(x);
	}

	template <typename T>
	typename List<T>::iterator List<T>::begin()
	{
		//_size == 0 同样适用
		return iterator(_pHead->_next);
	}

	template <typename T>
	typename List<T>::const_iterator List<T>::begin()const
	{
		//_size == 0 同样适用
		return const_iterator(_pHead->_next);
	}

	template <typename T>
	typename List<T>::iterator List<T>::end()
	{return iterator(_pHead);}

	template <typename T>
	typename List<T>::const_iterator List<T>::end()const
	{return const_iterator(_pHead);}

	template <typename T>
	typename List<T>::iterator List<T>::Insert(typename List<T>::iterator pos, const T &value)
	{
		// 空链表插入, 非空链表头插, 尾插, 中间插, 均可以使用下面一种方式
		Node* oldPos = pos._pNode;
		Node* oldPrev = pos._pNode->_prev;

		Node* newNode = new Node(value);
		newNode->_prev = oldPrev;
		newNode->_next = oldPos;
		oldPrev->_next = newNode;
		oldPos->_prev = newNode;
		++_size;
		return iterator(newNode);
	}

	template <typename T>
	typename List<T>::iterator List<T>::Erase(typename List<T>::iterator pos)
	{
		if(_size == 0)
		{
			std::cout << "Empty List, failed to erase!" << std::endl;
		}
		// 空链表删除, 非空链表头删, 尾删, 中间删, 均可以使用下面一种方式
		Node* cur = pos._pNode;
		Node* oldPrev = cur->_prev;
		Node* oldNext = cur->_next;

		oldPrev->_next = oldNext;
		oldNext->_prev = oldPrev;
		delete cur;
		--_size;
		return iterator(oldNext);
	}

	template <typename T>
	void List<T>::PushFront(const T &value)
	{
		Insert(begin(), value);
	}

	template <typename T>
	void List<T>::PushBack(const T &value)
	{
		Insert(end(), value);
	}

	template <typename T>
	void List<T>::PopFront()
	{
		Erase(begin());
	}

	template <typename T>
	void List<T>::PopBack()
	{
		Erase(iterator(_pHead->_prev));
	}

	
	template <typename T>
	void List<T>::Swap(List& x)
	{
		std::swap(_pHead, x._pHead);
		std::swap(_size, x._size);
	}

	template <typename T>
	bool List<T>::Empty()
	{
		return _size == 0;
	}

	template <typename T>
	void List<T>::Erase(iterator first, iterator last)
	{
		while(first != last)
		{
			first = Erase(first);
		}
	}

	template <typename T>
	void List<T>::Clear()
	{
		Erase(begin(), end());
	}

	template <typename T>
	size_t List<T>::Size()
	{
		return _size;
	}

	template <typename T>
	typename List<T>::Node* List<T>::GetNode()
	{
		return new typename List<T>::Node;
	}

	template <typename T>
	void List<T>::EmptyInit()//空链表(仅含一个头节点的链表)
	{
		_pHead = GetNode();
		_pHead->_prev = _pHead;
		_pHead->_next = _pHead;
		_size = 0;
	}
}