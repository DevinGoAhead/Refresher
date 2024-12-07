#include "Map.hpp"
#include "Set.hpp"


#include <iostream>
#include <string>

using namespace wxy;

void test1(Map<int, std::string>& mp)
{
	mp._rbtree.PrintInOrder();
	std::cout << std::endl;
	mp._rbtree.PrintLevelOrder();
	mp._rbtree.TreeHeight();
	std::cout << mp._rbtree.isValidRBTree() <<  std::endl;
}

void test2()
{

}

int main()
{
	Map<int, std::string> mp;
	//std::pair<int, std::string> a(1, "三");
	//mp._rbtree.Insert(a);//顶点
	mp.Insert(std::make_pair(3, "三"));
	mp.Insert(std::make_pair(5, "五"));
	mp.Insert(std::make_pair(6, "六"));
	mp.Insert(std::make_pair(7, "七"));
	mp.Insert(std::make_pair(8, "八"));
	mp.Insert(std::make_pair(9, "九"));
	mp.Insert(std::make_pair(10, "十"));
	mp.Insert(std::make_pair(11, "十一"));
	mp.Insert(std::make_pair(12, "十二"));
	mp.Insert(std::make_pair(13, "十三"));
	mp.Insert(std::make_pair(14, "十四"));
	mp.Insert(std::make_pair(16, "十六"));
	mp.Insert(std::make_pair(17, "十七"));

	//test1(mp);

	Map<int, std::string>::iterator iter = mp.begin();
	Map<int, std::string>::iterator endIter = mp.end();
	//std:: cout << (*iter).first << std::endl;
	for(; iter != endIter; ++iter)
	{
		std::cout << "("<< (*iter).first  << ", " << (*iter).second << ")" << " "; 
		if((*iter).first == 17)
			break;
	}

	std::cout << std::endl;

	mp[13] = "新十三";
	
	std:: cout << (*iter).first << std::endl;
	
	for(; iter != endIter; --iter) // iter != endIter 不合理, 仅为测试
	{
		std::cout << "("<< (*iter).first  << ", " << (*iter).second << ")" << " "; 
	}
	std::cout << std::endl;

	for(const auto& m : mp)
	{
		std::cout << "("<< m.first  << ", " << m.second << ")" << " ";
	}
	

}