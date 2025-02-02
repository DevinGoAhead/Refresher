#include "BitSet.hpp"
#include <string>
#include <tuple>

// 一些字符串哈希函数
struct BKDRHash
{
	size_t operator()(const std::string& str)
	{
		size_t hash = 0;
		for (auto ch : str)
		{
			hash *= 131;
			hash += ch;
		}
		return hash;
	}
};

struct APHash
{
	size_t operator()(const std::string& str)
	{
		unsigned int hash = 0;
		int i = 0;

		for (auto ch : str)
		{
			if ((i & 1) == 0)//判断ch是str的第奇数个字符或第偶数个字符
			{
				hash ^= ((hash << 7) ^ (ch) ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ (ch) ^ (hash >> 5)));
			}

			++i;
		}

		return hash;
	}
};

struct DJBHash
{
	size_t operator()(const std::string& str)
	{
		unsigned int hash = 5381;

		for (auto ch : str)
		{
			hash += (hash << 5) + ch;
		}

		return hash;
	}
};

struct JSHash
{
	size_t operator()(const std::string& s)
	{
		size_t hash = 1315423911;
		for (auto ch : s)
		{
			hash ^= ((hash << 5) + ch + (hash >> 2));
		}
		return hash;
	}
};

// class BloomFilter
// BitArrayRatio 表示 布隆过滤器长度 / 插入元素的个数(Num)
template<size_t Num, typename T = std::string, size_t BitArrayRatio = 5,
		typename... Hashes>
class BloomFilte
{
public:
	//BloomFilter() = default;
	void Set(const T& x)
	{
		size_t m = Num * BitArrayRatio;
		std::apply([this, m, x](const auto& ... hash){_bitSet.Set(hash(x) % m), ...;},
			constexpr sizeof ...(Hashes) == 0 ?
			std::make_tuple(BKDRHash(), APHash(), DJBHash(), JSHash()) :
			std::make_tuple(Hashes{}...));
	}
	
	bool Prob(const T& x)
	{
		size_t m = Num * BitArrayRatio;
		std::apply([this, m, x](const auto& ... hash) -> bool {return ... && _bitSet.Prob(hash(x) % m);},
			constexpr sizeof ...(Hashes) == 0 ?
			std::make_tuple(BKDRHash(), APHash(), DJBHash(), JSHash()) :
			std::make_tuple(Hashes{}...));
	}
private:
	BitSet<Num * BitArrayRatio> _bitSet;
};
