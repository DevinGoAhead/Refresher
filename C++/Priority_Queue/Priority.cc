#include "Priority.hpp"
#include <iostream>
using namespace wxy;

void Print(PriorityQueue<int, std::vector<int>,  greater<int>> pq)
{
	//std::cout << *list.begin() <<std::endl;
	while(!pq.Empty())
	{
		std::cout << pq.Top() << " ";
		pq.Pop();
	}
	std::cout << std::endl;
}
int main()
{
	PriorityQueue<int, std::vector<int>, greater<int>> pq;
	pq.Push(1);
	pq.Push(3);
	pq.Push(5);
	pq.Push(7);
	pq.Push(9);
	pq.Push(10);
	pq.Push(14);
	pq.Push(16);
	pq.Push(18);

	Print(pq);
}