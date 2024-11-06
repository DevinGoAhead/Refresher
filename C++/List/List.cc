#include "List.hpp"
using namespace wxy;

void Print(const List<int>& list)
{
	//std::cout << *list.begin() <<std::endl;
	for(const auto &e:list)
	{
		std::cout << e << " ";
	}

	std::cout << std::endl;
}
int main()
{
	List<int> lt1(5, 1);
	std::cout <<"lt1: "<< "size: "<<lt1.Size()<<std::endl;
	Print(lt1);

	lt1.PushBack(2);
	lt1.PushFront(0);
	std::cout <<"lt1: "<< "size: "<<lt1.Size()<<std::endl;
	Print(lt1);

	lt1.PopBack();
	lt1.PopFront();
	std::cout <<"lt1: "<< "size: "<<lt1.Size()<<std::endl;
	Print(lt1);

	lt1.Clear();
	std::cout <<"lt1: "<< "size: "<<lt1.Size()<<std::endl;
	Print(lt1);
	
	int i = 6;
	while(i--)
	{
		lt1.PushBack(i);
	}

	List<int> lt2(lt1);
	std::cout <<"lt2: "<< "size: "<<lt2.Size()<<std::endl;
	Print(lt2);

	List<int>::iterator iter1 = lt1.begin();
	List<int>::iterator iter2 = lt1.begin();
	++iter1;
	--iter2;
	--iter2;

	List<int> lt3(iter1, iter2);
	std::cout <<"it3: "<< "size: "<<lt3.Size()<<std::endl;
	Print(lt3);
	
	lt2.PushFront(6);
	lt2.PushFront(7);
	std::cout <<"lt2: "<< "size: "<<lt2.Size()<<std::endl;
	Print(lt2);
	List<int>::iterator iter3 = lt2.begin();
	List<int>::iterator iter4 = lt2.end();
	++iter3;
	++iter3;
	--iter4;
	--iter4;

	lt2.Erase(iter3, iter4);
	std::cout <<"lt2: "<< "size: "<<lt2.Size()<<std::endl;
	Print(lt2);

	


}