#include <iostream>
// #include <queue>
#include <vector>
#include <utility>
#include <cassert>
// #include <cstdlib>

namespace wxy
{	
	template<typename K>
	struct HashFunc_1 // hash 函数, 获取 关键码 的地址
	{
		K operator()(const K& key)
		{
			//return key % wxy::ch::_capacity;// 实现起来挺麻烦
			return key; // 这里做简化处理, 形式上使用哈希函数, 但实际不做任何处理
		}
	};

	unsigned long getNextPrime(unsigned long curCapacity) // 获取一个比 curCapacity 大的质数
	{
		assert(curCapacity < 4294967291ul);
	    static const unsigned long primeList[] =
		{
		  /*3ul,		6ul,		  9ul,*/
		  53ul,         97ul,         193ul,       389ul,       769ul,
		  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
		  1610612741ul, 3221225473ul, 4294967291ul
		};
		for(const auto& prime : primeList)
		{
			if(prime > curCapacity)
				return prime;
		}
		return 4294967291ul;// 通常不会到这里
	}

	namespace ch // closed hash
	{
		enum State // 标记节点的状态, 空位, 占位, 已删除
		{
			Empty,
			Exist,
			Delete
		};

		template<typename K, typename V>
		struct __HashEntry
		{
		public:
			__HashEntry(const std::pair<K,V>& kv = std::pair<K,V>()) :_kv(kv), _state(Empty)
			{}

		public:
			std::pair<K,V> _kv;
			State _state;
		};

		template<typename K, typename V, typename HashFunc = HashFunc_1<K>>
		class HashTable
		{
		public:
			using HashEntry = __HashEntry<K,V>;
			//using HashEntryPtr = HashEntry*;
		public:
			HashTable(std::vector<HashEntry> table = std::vector<HashEntry>()) : _table(table), _num(0)
			{
			}
			bool Insert(const std::pair<K,V>& kv);
			HashEntry* Find(const K& key);
			bool Erase(const K& key);
			void Print();
		private:
			std::vector<HashEntry> _table;
			size_t _num;// 这里指的是状态为 Exist 状态的HashEntry, 考虑到使用_size 会有歧义, 因此使用 _num
		};

		template<typename K, typename V, typename HashFunc>
		bool HashTable<K, V, HashFunc>::Insert(const std::pair<K,V>& kv)
		{
			if(Find(kv.first) != nullptr)
				return false;
			// 判断是否需要扩容
			size_t curCapacity = _table.capacity();
			if(curCapacity == 0 || (10 * _num) / curCapacity > 7)// 装载因子 > 0.7, 扩容
			{
				size_t newCapacity = getNextPrime(curCapacity); // 保持capacity 为质数
				HashTable<K, V, HashFunc> newTable;
				newTable._table.resize(newCapacity); // 将调用 __HashEntry 的默认构造函数, 将 newTable 元素设置为默认值

				// 将原 hash table 中的元素, 映射至 newTable
				// 如果是 curCapacity == 0 的情况下扩容的, 则这里不会将进入映射循环
				for(size_t i = 0; i < curCapacity; ++i)
				{
					if(_table[i]._state == Exist)
						newTable.Insert(_table[i]._kv); // 不会循环扩容, 会跳过扩容, 直接插入
				}
				_table.swap(newTable._table); // 交换迭代器(指针)
				curCapacity = newCapacity; // 映射完成后, 后续插入新元素时需要使用 curCapacity, 这里更新一下
				//std::cout << "test" << std::endl;
			}
			
			// 插入
			// HashFunc 只用一次, 使用匿名对象即可
			// 获取 posHash 应该直接在 HashFunc 中完成
			size_t posHash = HashFunc()(kv.first) % curCapacity; 
			//size_t posHash = kv.first % curCapacity; 

			for(size_t i_probe = 0; i_probe < curCapacity; ++i_probe)
			{
				size_t iHash = (posHash + i_probe * i_probe) % curCapacity; // 二次探测
				if(_table[iHash]._state != Exist)
				{
					_table[iHash]._kv = kv;
					_table[iHash]._state = Exist;
					++_num;
					return true;
				}
			}
			return false; // 通常不会到这里
		}

		template<typename K, typename V, typename HashFunc>
		//typename Hash<K, T, HashFunc>::HashEntry* Hash<K, T, HashFunc, KofT>::HashFind(const K& key)
		auto HashTable<K, V, HashFunc>::Find(const K& key) -> HashEntry*
		{
			if(_num == 0)
				return nullptr;

			size_t capacity = _table.capacity();
			// HashFunc 只用一次, 使用匿名对象即可
			// 获取 posHash 应该直接在 HashFunc 中完成
			size_t posHash = HashFunc()(key) % capacity;
			size_t i_probe = posHash;
			while( _table[i_probe]._state != Empty) // 说明该 key 在 哈希表中, 但是否在 _tabel[i_probe] 位置, 还需要进一步确定
			{
				if(_table[i_probe]._state == Exist && _table[i_probe]._kv.first == key)
					return &_table[i_probe];
				// 到这里, 
				// 或者 key 原来在 _table[i_probe] 位置, 但后来被删除了 
				// 也或许 _table[i_probe] 被其它数据占用了, key 被放到了其它位置上
				++i_probe;
				i_probe %= capacity; // i_probe < capacity
				if(i_probe == posHash)
					return nullptr;
			}
			return nullptr;// 通常不会到这里
		}
		template<typename K, typename V, typename HashFunc>
		bool HashTable<K, V, HashFunc>::Erase(const K& key)
		{
			HashEntry* delEntr = Find(key);
			if(delEntr == nullptr)
				return false;
			delEntr->_state = Delete;
			--_num;
			return true;
		}
		template<typename K, typename V, typename HashFunc>
		void HashTable<K, V, HashFunc>::Print()
		{
			size_t capacity = _table.capacity();
			std::cout <<  "size: " << _num << std::endl;
			for(size_t i = 0; i < capacity; ++i)
			{
				std::cout << "(" << _table[i]._kv.first << ", " << _table[i]._kv.second << ", " << _table[i]._state << ")";
			}
			std::cout << std::endl;
		}
	}// end of namespace ch

	namespace oh // open hash
	{
		template<typename K, typename V>
		struct __BucketNode
		{
		public:
			__BucketNode(const std::pair<K,V>& kv = std::pair<K,V>()) :_kv(kv), _next(nullptr)
			{}

		public:
			std::pair<K,V> _kv;
			__BucketNode<K, V>* _next;
		};

		template<typename K, typename V, typename HashFunc = HashFunc_1<K>>
		class HashTable
		{
		public:
			using BucketNode = __BucketNode<K, V>;
			//using HashEntryPtr = HashEntry*;
		public:
			HashTable(std::vector<BucketNode*> table = std::vector<BucketNode*>()) : _table(table), _size(0)
			{
			}
			bool Insert(const std::pair<K,V>& kv);
			BucketNode* Find(const K& key);
			bool Erase(const K& key);
			void Print();
		private:
			std::vector<BucketNode*> _table;
			size_t _size;
		};

		template<typename K, typename V, typename HashFunc>
		bool HashTable<K, V, HashFunc>::Insert(const std::pair<K,V>& kv)
		{
			if(Find(kv.first) != nullptr)
				return false;
			// 判断是否需要扩容
			// 扩容会导致数据在各个桶中重新分布
			size_t curCapacity = _table.capacity();
			if(curCapacity == _size) // 数据个数 与 桶个数相等, 则扩容
			{
				size_t newCapacity = getNextPrime(curCapacity); // 保持capacity 为质数
				HashTable<K, V, HashFunc> newTable;
				newTable._table.resize(newCapacity); // 将调用 __HashEntry 的默认构造函数, 将 newTable 元素设置为默认值

				// 将原 hash table 中的元素, 映射至 newTable
				// 如果是 curCapacity == 0 的情况下扩容的, 则这里不会将进入映射循环
				for(size_t i = 0; i < curCapacity; ++i)
				{
					BucketNode* cur = _table[i];
					while( cur != nullptr)
					{
						newTable.Insert(cur->_kv); // 不会循环扩容, 会跳过扩容, 直接插入
						cur = cur->_next;
					}
				}
				_table.swap(newTable._table); // 交换迭代器(指针)
				curCapacity = newCapacity; // 映射完成后, 后续插入新元素时需要使用 curCapacity, 这里更新一下
				//std::cout << "test" << std::endl;
			}
			// 插入, 头插
			// HashFunc 只用一次, 使用匿名对象即可
			// 获取 posHash 应该直接在 HashFunc 中完成
			size_t posHash = HashFunc()(kv.first) % curCapacity; 
			
			BucketNode* oldFirst = _table[posHash]; // 无论是否为 nullptr
			_table[posHash] = new BucketNode(kv);
			_table[posHash]->_next = oldFirst;

			// // 这样也行, 可读性更强
			// BucketNode* newFirst = new BucketNode(kv);
			// _table[i_probe] = newFirst;
			// newFirst->_next =  oldFirst;
			++_size;
			return true; // 通常不会到这里
		}

		template<typename K, typename V, typename HashFunc>
		//typename Hash<K, T, HashFunc>::HashEntry* Hash<K, T, HashFunc, KofT>::HashFind(const K& key)
		auto HashTable<K, V, HashFunc>::Find(const K& key) -> BucketNode*
		{
			if(_size == 0)
				return nullptr;

			size_t capacity = _table.capacity();
			// HashFunc 只用一次, 使用匿名对象即可
			// 获取 posHash 应该直接在 HashFunc 中完成
			size_t posHash = HashFunc()(key) % capacity;
			BucketNode* cur = _table[posHash];
			while( cur != nullptr) // 说明该 key 在 该 bucket 中
			{
				if(cur->_kv.first == key)
					return cur;
				cur = cur->_next;
			}
			return nullptr;// 不在 hash table 中
		}

		template<typename K, typename V, typename HashFunc>
		bool HashTable<K, V, HashFunc>::Erase(const K& key)
		{
			if(_size == 0)
				return false;

			size_t capacity = _table.capacity();
			// HashFunc 只用一次, 使用匿名对象即可
			// 获取 posHash 应该直接在 HashFunc 中完成
			size_t posHash = HashFunc()(key) % capacity;
			size_t i_probe = posHash;
			
			BucketNode* cur = _table[i_probe];
			BucketNode* prev = cur;
			while( cur != nullptr) // 说明该 key 在该 bucket 中
			{
				if(cur->_kv.first == key)
				{
					if(cur == _table[i_probe]) // cur 为第一个节点, 无论该桶中是否仅有 1 个节点
					{
						_table[i_probe] = cur->_next;
						delete cur;
					}
					else //无论cur 是否为最后一个节点 (此时已经排除了仅有一个点的情况)
					{
						prev->_next = cur->_next;
						delete cur;
					}
					--_size;
					return true;
				}
				prev = cur;
				cur = cur->_next;
			}
			return false;// 不在 hash table 中
		}
		template<typename K, typename V, typename HashFunc>
		void HashTable<K, V, HashFunc>::Print()
		{
			size_t capacity = _table.capacity();
			std::cout <<  "size: " << _size << std::endl;
			for(size_t i = 0; i < capacity; ++i)
			{
				BucketNode* cur = _table[i];
				if(cur == nullptr) std::cout << "nullptr" << std::endl;
				else
				{
					while(cur != nullptr)
					{
						std::cout << "(" <<cur->_kv.first << ", " << cur->_kv.second << ")";
						cur = cur->_next;
					}
					std::cout << std::endl;
				}
			}
			std::cout << std::endl;
		}
	}// end of namespace oh
}// end of namespace wxy