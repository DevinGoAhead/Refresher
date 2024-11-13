#include <cstddef>//定义了size_t
#include <vector>
#include <algorithm>

namespace wxy
{
	/* less 和 greater 都是按照STL 的逻辑来的, less 用于构建大堆, greater 用于构建大堆
	 * less 升序, greater 降序
	 * 有点不符合直觉...
	 */
	template<typename T>
	struct less//仿函数
	{
		bool operator()(const T left, const T right)const
		{
			return left > right;
		}
	};

	template<typename T>
	struct greater//仿函数
	{
		bool operator()(const T left, const T right)const
		{
			return left < right;
		}
	};

	//class PriorityQueue
	template<typename T, typename Container = std::vector<T>, typename Compare = less<T>>//默认构建大堆
	class PriorityQueue
	{
	public:
		void Push(const T &value);//PushBack
		void Pop();//Remove the one with the highest(lowest) value
		size_t Size()const;
		const T& Top()const;//the highest(lowest) value
		bool Empty()const;
	private:
		void AdjustToUp(size_t child);//向上调整算法
		void AdjustToDown(size_t parent);//向下调整算法
	private:
		Container _con;
	};

	template<typename T, typename Container, typename Compare>
	void PriorityQueue<T, Container, Compare>::Push(const T &value)
	{
		_con.push_back(value);
		AdjustToUp(_con.size() - 1);
	}

	template<typename T, typename Container, typename Compare>
	void  PriorityQueue<T, Container, Compare>::Pop()
	{
		std::swap(_con[0], _con[_con.size() - 1]);
		_con.pop_back();
		AdjustToDown(0);
	}

	template<typename T, typename Container, typename Compare>
	size_t PriorityQueue<T, Container, Compare>::Size()const
	{
		return _con.size();
	}

	template<typename T, typename Container, typename Compare>
	const T& PriorityQueue<T, Container, Compare>::Top()const
	{
		return _con.front();
	}

	template<typename T, typename Container, typename Compare>
	bool PriorityQueue<T, Container, Compare>::Empty()const
	{
		return _con.empty();
	}

	template<typename T, typename Container, typename Compare>
	void PriorityQueue<T, Container, Compare>::AdjustToUp(size_t child)
	{
		Compare cmp;
		while(child > 0)
		{
			size_t parent = (child - 1) / 2;
			if(cmp(_con[child], _con[parent]))
				std::swap(_con[child], _con[parent]);
			else//上面的数据更大
				break;
			child = parent; 
		}
	}

	template<typename T, typename Container, typename Compare>
	void PriorityQueue<T, Container, Compare>::AdjustToDown(size_t parent)
	{
		Compare cmp;
		size_t size = _con.size();
		size_t maxChild = 2 * parent + 1;
		while(maxChild < size)
		{
			
			if(maxChild + 1 < size && cmp(_con[maxChild + 1],  _con[maxChild]))
				++maxChild;
			if(cmp(_con[maxChild], _con[parent]))
				std::swap(_con[maxChild], _con[parent]);
			else//下面的数据更小
				break;
			parent = maxChild;
			maxChild = 2 * parent + 1;
		}
	}
}