#include <cstddef>//定义了size_t
#include <vector>
#include <algorithm>

namespace wxy
{
	template<typename T, typename Container = std::vector<T>>//默认构建大堆
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

	template<typename T, typename Container>
	void PriorityQueue<T, Container>::Push(const T &value)
	{
		_con.push_back(value);
		AdjustToUp(_con.size() - 1);
	}

	template<typename T, typename Container>
	void  PriorityQueue<T, Container>::Pop()
	{
		std::swap(_con[0], _con[_con.size() - 1]);
		_con.pop_back();
		AdjustToDown(0);
	}

	template<typename T, typename Container>
	size_t PriorityQueue<T, Container>::Size()const
	{
		return _con.size();
	}

	template<typename T, typename Container>
	const T& PriorityQueue<T, Container>::Top()const
	{
		return _con.front();
	}

	template<typename T, typename Container>
	bool PriorityQueue<T, Container>::Empty()const
	{
		return _con.empty();
	}

	template<typename T, typename Container>
	void PriorityQueue<T, Container>::AdjustToUp(size_t child)
	{
		while(child > 0)
		{
			size_t parent = (child - 1) / 2;
			if(_con[child] > _con[parent])
				std::swap(_con[child], _con[parent]);
			else//上面的数据更大
				break;
			child = parent; 
		}
	}

	template<typename T, typename Container>
	void PriorityQueue<T, Container>::AdjustToDown(size_t parent)
	{
		size_t size = _con.size();
		size_t maxChild = 2 * parent + 1;
		while(maxChild < size)
		{
			
			if(maxChild + 1 < size && _con[maxChild + 1] > _con[maxChild])
				++maxChild;
			if(_con[maxChild] > _con[parent])
				std::swap(_con[maxChild], _con[parent]);
			else//下面的数据更小
				break;
			parent = maxChild;
			maxChild = 2 * parent + 1;
		}
	}
}