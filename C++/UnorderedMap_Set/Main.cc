#include "UdSet.hpp"
#include "UdMap.hpp"

#include <iostream>
#include <string>

using namespace wxy;
//using namespace wxy::ch;
//using namespace wxy::oh;

int main()
{
	// UnorderedMap<int, std::string> udMp;
	// udMp.Insert(std::make_pair(3,"三"));
	// udMp.Insert(std::make_pair(5, "五"));
	// udMp.Insert(std::make_pair(6, "六"));
	// udMp.Insert(std::make_pair(53, "五十三"));
	// udMp.Insert(std::make_pair(8, "八"));
	// udMp.Insert(std::make_pair(9, "九"));
	// udMp.Insert(std::make_pair(106, "一百零六"));
	// udMp.Insert(std::make_pair(11, "十一"));
	// udMp.Insert(std::make_pair(12, "十二"));
	// udMp.Insert(std::make_pair(13, "十三"));
	// udMp.Insert(std::make_pair(14, "十四"));
	// udMp.Insert(std::make_pair(159, "一百五十九"));
	// udMp.Insert(std::make_pair(17, "十七"));
	// udMp.Print();
	// std::cout << std::endl;
	// udMp.PrintByIter();
	// // udMp.Erase(53);
	// // udMp.Insert(std::make_pair(212, "二百一十二"));
	// // udMp.Print();

	UnorderedSet<int> udSt;
	udSt.Insert(3);
	udSt.Insert(5);
	udSt.Insert(6);
	udSt.Insert(53);
	udSt.Insert(8);
	udSt.Insert(9);
	udSt.Insert(106);
	udSt.Insert(11);
	udSt.Insert(12);
	udSt.Insert(13);
	udSt.Insert(14);
	udSt.Insert(159);
	udSt.Insert(17);
	//udSt.Print();
	std::cout << std::endl;
	udSt.PrintByIter();
	// udSt.Erase(53);
	// udSt.Insert(std::make_pair(212, "二百一十二"));
	// udSt.Print();
}