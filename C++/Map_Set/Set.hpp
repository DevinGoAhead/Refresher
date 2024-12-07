#include "RBTree.hpp"

namespace wxy
{
	template<typename K>
	struct SetKofT
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};

	template<typename K>
	class Set
	{
	public:
		using iterator = typename RBTree<K, K, SetKofT<K>>::iterator; // typename 限定, 表示其是 iterator 是类型, 不是变量
		using const_iterator = typename RBTree<K, K, SetKofT<K>>::const_iterator;

	public:
		iterator begin(){return _rbtree.begin();}
		iterator end(){return _rbtree.end();}
		const_iterator begin()const
		{return _rbtree.begin();}
		const_iterator end()const
		{return _rbtree.end();}
		
		std::pair<iterator, bool> Insert(const K& key){return _rbtree.Insert(key);}
	
	//private:
		RBTree<K, K, SetKofT<K>> _rbtree;
	};
}