#include "RBTree.hpp"

#include <iostream>
#include <string>

using namespace wxy;

int main()
{
	RBTree<int, std::string> rbTree;
	rbTree.Insert(std::make_pair(3, "三"));//顶点
	rbTree.Insert(std::make_pair(5, "五"));
	rbTree.Insert(std::make_pair(6, "六"));
	// rbTree.Insert(std::make_pair(7, "七"));
	// rbTree.Insert(std::make_pair(8, "八"));
	// rbTree.Insert(std::make_pair(9, "九"));
	// rbTree.Insert(std::make_pair(10, "十"));
	// rbTree.Insert(std::make_pair(11, "十一"));
	// rbTree.Insert(std::make_pair(12, "十二"));
	// rbTree.Insert(std::make_pair(13, "十三"));
	// rbTree.Insert(std::make_pair(14, "十四"));
	// rbTree.Insert(std::make_pair(16, "十六"));
	// rbTree.Insert(std::make_pair(17, "十七"));

	// rbTree.PrintInOrder();
	// std::cout << std::endl;
	rbTree.PrintLevelOrder();
	rbTree.TreeHeight();
	rbTree.isValidRBTree();
}