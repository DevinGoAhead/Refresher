#include "Hash.hpp"

using namespace wxy;
using namespace wxy::ch;// 为什么这里不需要限定 HashTable<K, std::pair<K, V>, MapKofT<K,V>>::iterator 的作用域?
//using namespace wxy::oh; // 而 这里要限定 HashTable<K, std::pair<K, V>, MapKofT<K,V>>::iterator 的作用域?

namespace wxy
{
	template<typename K, typename V>
	struct MapKofT
	{
		const K& operator()(const std::pair<K, V>& kv)
		{
			return kv.first;
		}
	};

	template<typename K, typename V>
	class UnorderedMap
	{
	public:
		using iterator = typename HashTable<K, std::pair<K, V>, MapKofT<K,V>>::iterator; 
		//using iterator = typename wxy::oh::HashTable<K, std::pair<K, V>, MapKofT<K,V>>::iterator; 

	public:
		iterator begin() {return _hashTable.begin();}
		iterator end() {return _hashTable.end();}

		std::pair<iterator, bool> Insert(const std::pair<K, V>& kv) {return _hashTable.Insert(kv);}
		V& operator[](const K& key)
		{
			auto result = _hashTable.Insert(std::make_pair(key, V()));
			return (*result).second;
		}

		bool Erase(const K& key) {return _hashTable.Erase(key);}
		void Print() {_hashTable.Print();}

		void PrintByIter()
		{
			for (iterator iter = begin(); iter != end(); ++iter)
			{
				std::cout << "(" << (*iter).first << ", " << (*iter).second << ")";
				// std::cout << "(" <<(*iter).first << ", " << cur->_kv.second << ")";
			}
		}

	private:
		HashTable<K, std::pair<K, V>, MapKofT<K, V>> _hashTable;
		//wxy::oh::HashTable<K, std::pair<K, V>, MapKofT<K, V>> _hashTable;
	};
}