#include "Hash.hpp"

using namespace wxy;
using namespace wxy::ch;// 为什么这里不需要限定 HashTable<K, std::pair<K, V>, MapKofT<K,V>>::iterator 的作用域?
//using namespace wxy::oh; // 而 这里要限定 HashTable<K, std::pair<K, V>, MapKofT<K,V>>::iterator 的作用域?

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
	class UnorderedSet
	{
	public:
		using iterator = typename HashTable<K, K, SetKofT<K>>::iterator; 
		//using iterator = typename wxy::oh::HashTable<K, K, SetKofT<K>>::iterator; 

	public:
		iterator begin() {return _hashTable.begin();}
		iterator end() {return _hashTable.end();}

		std::pair<iterator, bool> Insert(const K& key) {return _hashTable.Insert(key);}

		bool Erase(const K& key) {return _hashTable.Erase(key);}
		void Print() {_hashTable.Print();}

		void PrintByIter()
		{
			for (iterator iter = begin(); iter != end(); ++iter)
			{
				std::cout <<(*iter) << " ";
			}
		}

	private:
		HashTable<K, K, SetKofT<K>> _hashTable;
		//wxy::oh::HashTable<K, K, SetKofT<K>> _hashTable;
	};
}