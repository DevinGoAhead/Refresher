#include <vector>

// 允许映射的数的范围为[0, MaxNum]
template<size_t MaxNum>
class BitSet
{
public:
	BitSet()
	{
		_chs.resize(MaxNum / 8 + 1); // MaxNum / 8 个char 的基础上再 + 一个 char
	}

	void Set(int x)
	{
		// x 映射到第 N 个 char 的第 n 个 bit 位
		size_t N = x / 8;
		size_t n = x % 8;
		_chs[N] |= (1 << n); // 将该 bit 位置为1
	}

	void Reset(int x)
	{
		// x 映射到第 N 个 char 的第 n 个 bit 位
		size_t N = x / 8;
		size_t n = x % 8;
		_chs[N] &= ~(1 << n); // 将该 bit 位置为0
	}

	bool Prob(int x)
	{
		// x 映射的位置为第 N 个 char 的第 n 个 bit 位
		size_t N = x / 8;
		size_t n = x % 8;

		return _chs[N] & (1 << n); // 只有 映射的比特位为1, 返回值才为 true
	}
private:
	std::vector<char> _chs;
};