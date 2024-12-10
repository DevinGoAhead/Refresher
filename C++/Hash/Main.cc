#include "Hash.hpp"

#include <iostream>
#include <string>

using namespace wxy;
using namespace wxy::ch;
//using namespace wxy::oh;

int main()
{
	HashTable<int, std::string> hs;
	hs.Insert(std::make_pair(3, "三"));
	hs.Insert(std::make_pair(5, "五"));
	hs.Insert(std::make_pair(6, "六"));
	hs.Insert(std::make_pair(53, "五十三"));
	hs.Insert(std::make_pair(8, "八"));
	hs.Insert(std::make_pair(9, "九"));
	hs.Insert(std::make_pair(106, "一百零六"));
	hs.Insert(std::make_pair(11, "十一"));
	hs.Insert(std::make_pair(12, "十二"));
	hs.Insert(std::make_pair(13, "十三"));
	hs.Insert(std::make_pair(14, "十四"));
	hs.Insert(std::make_pair(159, "一百五十九"));
	hs.Insert(std::make_pair(17, "十七"));
	hs.Print();
	hs.Erase(53);
	hs.Insert(std::make_pair(212, "二百一十二"));
	hs.Print();
}