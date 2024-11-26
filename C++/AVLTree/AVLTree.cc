#include "AVLTree.hpp"

#include <iostream>
#include <string>

using namespace wxy;

int main()
{
	AVLTree<int, std::string> avlTree;
	avlTree.Insert(std::make_pair(3, "三"));
	avlTree.Insert(std::make_pair(5, "五"));
	avlTree.Insert(std::make_pair(6, "六"));
	avlTree.Insert(std::make_pair(7, "七"));
	avlTree.Insert(std::make_pair(8, "八"));
	avlTree.Insert(std::make_pair(9, "九"));
	avlTree.Insert(std::make_pair(10, "十"));
	avlTree.Insert(std::make_pair(11, "十一"));
	avlTree.Insert(std::make_pair(12, "十二"));
	avlTree.Insert(std::make_pair(13, "十三"));
	avlTree.Insert(std::make_pair(14, "十四"));
	avlTree.Insert(std::make_pair(16, "十六"));
	avlTree.Insert(std::make_pair(17, "十七"));

	avlTree.PrintInOrder();
	std::cout << std::endl;
	avlTree.PrintLevelOrder();
	std::cout << avlTree.isBalance() << std::endl;
}