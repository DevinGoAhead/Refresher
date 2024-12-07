#include "RBTree.hpp"

namespace wxy
{
	template<typename K, typename V>
	struct MapKofT
	{
		const K& operator()(const std::pair<K,V>& kv)
		{
			return kv.first;
		}
	};

	template<typename K, typename V>
	class Map
	{
	public:
		using iterator = typename RBTree<K, std::pair<K, V>, MapKofT<K,V>>::iterator; // typename 限定, 表示其是 iterator 是类型, 不是变量
		using  const_iterator = typename RBTree<K, std::pair<K, V>, MapKofT<K,V>>::const_iterator;

	public:
		iterator begin(){return _rbtree.begin();}
		iterator end(){return _rbtree.end();}
		const_iterator begin()const
		{return _rbtree.begin();}
		const_iterator end()const
		{return _rbtree.end();}
		
		std::pair<iterator, bool> Insert(const std::pair<K,V>& kv){return _rbtree.Insert(kv);}
		
		V& operator[](const K& key)
		{
			auto result = _rbtree.Insert(std::make_pair(key, V()));
			return result.first->second;
		}
	
	//private:
		RBTree<K, std::pair<K, V>, MapKofT<K,V>> _rbtree;
	};
}